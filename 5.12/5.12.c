#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * const *argv)
{
    int pid, status;

    if (argc < 2) {
        printf("Too few arguments\n");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork\n");
        return 2;
    }

    if (pid == 0) { /*Child process*/
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        return 1;
    }

    wait(&status);
    if (WIFEXITED(status)) {
        printf("Exited %d\n", WEXITSTATUS(status));
    } else {
        printf("Killed %d\n", WTERMSIG(status));
    }
}
