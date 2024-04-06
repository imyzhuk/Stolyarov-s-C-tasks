#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "../modules/strings_utils.h"
#include "../modules/stdin_handler.h"

void push_string_to_strings_array(char ***array, char *str)
{
    char **tmp;
    int array_length;

    if (!*array) {
        tmp = malloc(sizeof(char *) * 2);
        tmp[0] = str;
        tmp[1] = NULL;
        *array = tmp;
        return;
    }

    array_length = get_array_length(*array);
    tmp = malloc(sizeof(char *) * (array_length + 2));
    copy_array(tmp, *array, array_length);
    tmp[array_length] = str;
    tmp[array_length + 1] = NULL;
    
    free(*array);
    *array = tmp;
}

void make_cmd_line_from(struct item *list, char ***cmd_line)
{
    int item_index;
    struct item *tmp;
    for (tmp = list, item_index = 0; tmp; tmp = tmp->next, item_index++)
    {
        push_string_to_strings_array(cmd_line, tmp->str);
    }
}

void change_dir(char **cmd_line)
{
    int result;

    if (get_array_length(cmd_line) > 2) {
        fprintf(stderr, "Too many arguments\n");
        return;
    }

    if (cmd_line[1]) {
        result = chdir(cmd_line[1]);
    } else if (getenv("HOME")) {
        chdir(getenv("HOME"));
        return;
    } else {
        fprintf(stderr, "I don't know where is your home :-(\n");
        return;
    }

    if (result == -1)
        perror(cmd_line[1]);
}

void exec_str(struct item *list)
{
    int pid;
    char **cmd_line = NULL;

    make_cmd_line_from(list, &cmd_line);
    if (are_strings_same(cmd_line[0], "cd")) {
        change_dir(cmd_line);
        free(cmd_line);
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork\n");
    }

    if (pid == 0) {
        execvp(cmd_line[0], cmd_line);
        perror(cmd_line[0]);
        exit(1);
    }

    wait(NULL); 
    free(cmd_line);
}

int main() 
{
    struct item *list = NULL;
    read_from_stdin(&list, exec_str);
}
