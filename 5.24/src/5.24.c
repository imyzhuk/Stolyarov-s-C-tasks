#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include "../modules/strings_utils.h"
#include "../modules/stdin_handler.h"

void sigchld_handler(int s)
{
    int pid;
    signal(SIGCHLD, sigchld_handler);
    do {
        pid = waitpid(-1, NULL, WNOHANG);
    } while (pid > 0);
}

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

void prepare_io_tokens_for_cmdline(char **cmdline, int *error)
{
    int fd;
    int in_signs_count = 0;
    int out_signs_count = 0;
    for (int i = 0; cmdline[i]; i++)
    {
        if (in_signs_count > 1 || out_signs_count > 1) {
            *error = -1;
            fprintf(stderr, "Too many tokens: <, >, >>\n");
            return;
        }

        if (
            !(are_strings_same(cmdline[i], "<")
                || are_strings_same(cmdline[i], ">")
                || are_strings_same(cmdline[i], ">>")
            )
        ) {
            continue;
        }

        if (!cmdline[i + 1]) {
            *error = -1;
            fprintf(stderr, "There isn't a file name after one of token: <, >, >>\n");
            return;
        }

        if (are_strings_same(cmdline[i], "<")) {
            in_signs_count++;
            fd = open(cmdline[i + 1], O_RDONLY);
            if (fd == -1) {
                *error = -1;
                perror(cmdline[i + 1]);
                return;
            }
            dup2(fd, 0);
        } else if (are_strings_same(cmdline[i], ">")) {
            out_signs_count++;
            fd = open(cmdline[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1) {
                *error = -1;
                perror(cmdline[i + 1]);
                return;
            }
            dup2(fd, 1);
        } else {
            out_signs_count++;
            fd = open(cmdline[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd == -1) {
                *error = -1;
                perror(cmdline[i + 1]);
                return;
            }
            dup2(fd, 1);
        }

        close(fd);
        cmdline[i] = NULL;
        continue;
    }
}

void exec_str(struct item *list)
{
    int pid, got_pid;
    char **cmdline = NULL;
    int is_execution_background_mode, error;
    sigset_t mask_chld;
    sigemptyset(&mask_chld);
    sigaddset(&mask_chld, SIGINT);
    
    sigprocmask(SIG_SETMASK, &mask_chld, NULL);

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
        prepare_io_tokens_for_cmdline(cmdline, &error);
        if (error == -1) {
            exit(2);
        }
        execvp(cmdline[0], cmdline);
        perror(cmdline[0]);
        exit(1);
    }
    if (!is_execution_background_mode) {
        signal(SIGCHLD, SIG_DFL);
        sigprocmask(SIG_UNBLOCK, &mask_chld, NULL);
        do {
            got_pid = wait(NULL);
        } while (got_pid != pid);
        free(cmdline);
    }
    
    signal(SIGCHLD, sigchld_handler);
    sigprocmask(SIG_UNBLOCK, &mask_chld, NULL);
}

int main() 
{
    struct item *list = NULL;
    read_from_stdin(&list, exec_str);
}
