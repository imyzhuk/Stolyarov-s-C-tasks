#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int get_rows_count(const char *text, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (text[i] == '\n')
            count++;
    }
    return count;
}
int main(int argc, char **argv)
{
    enum { buf_size = 4096 };
    char buf[buf_size];
    const char args_error[] = "Too few arguments\n";
    int fd;
    int read_bytes_count, rows_count = 0;
    char last_smbl;

    if (argc < 2) {
        write(1, args_error, sizeof(args_error));
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        return 2;
    }

    while ((read_bytes_count = read(fd, buf, buf_size)))
    {
        if (read_bytes_count == -1) {
            perror(argv[1]);
            return 3;
        }

        rows_count += get_rows_count(buf, read_bytes_count);
        last_smbl = buf[read_bytes_count - 1];
    }

    if (last_smbl != '\n') {
        rows_count++;
    }

    printf("%d\n", rows_count);
}