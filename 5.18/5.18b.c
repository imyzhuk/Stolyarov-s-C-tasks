#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int fd, pid;   
    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror(argv[1]);
        return 2;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork\n");
        return 3;
    }

    if (pid == 0) {
        dup2(fd, 0);
        close(fd);
        execvp(argv[2], argv + 2);
        perror(argv[2]);
        exit(1);
    }

    wait(NULL);
}
