#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <unistd.h> //for lseek64()
#include <stdio.h> //for printf(), perror(), fprintf()
#include <fcntl.h> //for open()

int main(int argc, char **argv)
{
    char *filename;
    const char args_error[] = "Too few arguments\n";
    int fd;
    long long file_size;

    if (argc < 2) {
        fprintf(stderr, args_error);
        return 1;
    }
  
    filename = argv[1];
    fd = open(filename, O_RDONLY);

    if(fd == -1) {
        perror(filename);
        return 2;
    }

    file_size = lseek64(fd, 0, SEEK_END);
    printf("%lld\n", file_size);
}