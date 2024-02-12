#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int number, digits_count, c;
    int file_desc;
    const char args_error[] = "Too few arguments\n";

    if (argc < 2) {
        fprintf(stderr, args_error);
        return 1;
    }

    file_desc = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(file_desc == -1) {
        perror(argv[1]);
        return 2;
    }

    while ((c = getchar()) != EOF)
    {
        if (c == '-' || (c >= '0' && c <= '9')) {
            ungetc(c, stdin);
            digits_count = scanf("%d", &number);
            if (digits_count) {
                write(file_desc, &number, sizeof(number));
            }
        }
    }
}