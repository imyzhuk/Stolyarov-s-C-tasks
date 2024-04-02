#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <malloc.h>

struct item
{
    char *str;
    int pid;
    struct item *next;
};

int are_strings_same(const char *first_str, const char *second_str)
{
    enum {
        strings_are_not_same = 0,
        strings_are_same = 1
    };
    int i;

    for (i = 0; first_str[i] != '\0' || second_str[i] != '\0'; i++)
    {
        if (first_str[i] != second_str[i]) {
            return strings_are_not_same;
        }
    }

    if (first_str[i] == '\0' && second_str[i] == '\0') {
        return strings_are_same;
    }

    return strings_are_not_same;
}

void create_new_item_for_list(struct item **list, char *str, int pid)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->str = str;
    tmp->pid = pid;
    *list = tmp;
}

void add_item_to_list(struct item **list, char *str, int pid)
{
    struct item *last = *list;
    if (!*list) {
        create_new_item_for_list(list, str, pid);
        return;
    }

    while (last->next)
    {
        last = last->next;
    }
    

    create_new_item_for_list(&last->next, str, pid);
}

char *search_process_by_id(struct item *list, int pid)
{
    while (list)
    {
        if (list->pid == pid) {
            return list->str;
        }

        list = list->next;
    }

    return NULL;
};

int main(int argc, char **argv)
{
    int pid, exited_process_id, status, ps_count = 0;
    int cmd_arg_index, start_cmdline;
    struct item *list = NULL;
    char pattern[] = ";;";

    if (argc < 2) {
        printf("Too few arguments\n");
        return 1;
    }

    for (cmd_arg_index = 1, start_cmdline = 1; cmd_arg_index < argc; cmd_arg_index++)
    {
        if (are_strings_same(argv[cmd_arg_index], pattern)) {
            argv[cmd_arg_index] = NULL;
            pid = fork();

            if (pid == -1) {
                perror(argv[start_cmdline]);
            }

            if (pid > 0) {
                ps_count++;
                add_item_to_list(&list, argv[start_cmdline], pid);
            }

            if (pid == 0) { /*Child process*/
                execvp(argv[start_cmdline], argv + start_cmdline);
                perror(argv[start_cmdline]);
                return 1;
            }

            start_cmdline = cmd_arg_index + 1;
        }
    }

    if (cmd_arg_index > start_cmdline) {
        pid = fork();

        if (pid == -1) {
            perror(argv[start_cmdline]);
        }

        if (pid > 0) {
            ps_count++;
            add_item_to_list(&list, argv[start_cmdline], pid);
        }

        if (pid == 0) { /*Child process*/
            execvp(argv[start_cmdline], argv + start_cmdline);
            perror(argv[start_cmdline]);
            return 1;
        }
    }

    for (int i = 0; i < ps_count; i++)
    {
        exited_process_id = wait(&status);
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
            char *a = search_process_by_id(list, exited_process_id);
            printf("%s\n", a);
        }
    }  
}
