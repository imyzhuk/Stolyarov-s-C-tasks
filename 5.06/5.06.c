#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int get_str_length(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

void encrypt(unsigned char *str, int len, char *key)
{
    int key_length = get_str_length(key);

    for (int i = 0; i < len; i += key_length)
    {
        for (int j = 0; j < key_length; j++)
        {
            str[i + j] ^= key[j];
        }
    }
}

int main(int argc, char **argv)
{
    enum { buf_size = 4096 };
    unsigned char buf[buf_size];
    const char few_args_error[] = "Too few arguments\n";
    const char many_args_error[] = "Too many arguments\n";
    char *file_name, *key;
    int read_bytes_count, fd;

    if (argc < 3) {
        write(1, few_args_error, sizeof(few_args_error));
        return 1;
    }

    if (argc > 3) {
        write(1, many_args_error, sizeof(many_args_error));
        return 2;
    }

    file_name = argv[1];
    key = argv[2];

    fd = open(file_name, O_RDWR);
    if (fd == -1) {
        perror(file_name);
        return 3;
    }

    while ((read_bytes_count = read(fd, buf, buf_size)))
    {
        if (read_bytes_count == -1) {
            perror(argv[1]);
            return 4;
        }

        encrypt(buf, read_bytes_count, key);
        lseek64(fd, read_bytes_count * -1, SEEK_CUR);
        write(fd, buf, read_bytes_count);  
    }
}