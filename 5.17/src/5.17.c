#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "../modules/strings_utils.h"
#include "../modules/stdin_handler.h"

void make_cmdline_from(struct item *list, char ***cmdline)
{
    int item_index;
    struct item *tmp;
    for (tmp = list, item_index = 0; tmp; tmp = tmp->next, item_index++)
    {
        push_string_to_strings_array(cmdline, tmp->str);
    }
}

void change_dir(char **cmdline)
{
    int result;

    if (get_array_length(cmdline) > 2) {
        fprintf(stderr, "Too many arguments\n");
        return;
    }

    if (cmdline[1]) {
        result = chdir(cmdline[1]);
    } else if (getenv("HOME")) {
        chdir(getenv("HOME"));
        return;
    } else {
        fprintf(stderr, "I don't know where is your home :-(\n");
        return;
    }

    if (result == -1)
        perror(cmdline[1]);
}

void handle_ampersand_in_cmdline(char **cmdline, int *is_execution_background_mode, int *error)
{
    *error = 0;
    *is_execution_background_mode = 0;
    for (int i = 0; cmdline[i]; i++)
    {
        if (*is_execution_background_mode) {
            *error = -1;
            return;
        }

        if(are_strings_same(cmdline[i], "&")) {
            *is_execution_background_mode = 1;
            cmdline[i] = NULL;
        }
    }
}

void exec_str(struct item *list)
{
    int pid, got_pid;
    char **cmdline = NULL;
    int is_execution_background_mode, error;

    make_cmdline_from(list, &cmdline);

    handle_ampersand_in_cmdline(cmdline, &is_execution_background_mode, &error);
    if (error == -1) {
        fprintf(stderr, "There are arguments after '&'\n");
        free(cmdline);
        return;
    }
    
    if (are_strings_same(cmdline[0], "cd")) {
        change_dir(cmdline);
        free(cmdline);
        return;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork\n");
        free(cmdline);
        return;
    }

    if (pid == 0) { /*Child process actions*/
        execvp(cmdline[0], cmdline);
        perror(cmdline[0]);
        exit(1);
    }

    if (!is_execution_background_mode) {
        do {
            got_pid = wait(NULL);
        } while (got_pid != pid);
        free(cmdline);
    }
}

int main() 
{
    struct item *list = NULL;
    read_from_stdin(&list, exec_str);
}
