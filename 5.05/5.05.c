#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int get_whole_blocks_count(int all_blocks_size, int block_size)
{
    return all_blocks_size / block_size;
}

void fill_str_by_smbl(char *str, int len, char smbl)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = smbl;
    }
}

int is_digit(char smbl)
{
    if ('0' <= smbl && smbl <= '9')
        return 1;
    else
        return 0;
}

int get_str_length(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

void convert_str_to_int(const char *str, int *error, int *value)
{
    int str_length = get_str_length(str);
    int num = 0, is_negative_num = 0;
    char curr_smbl;

    *error = 0;

    for (int i = 0; i < str_length; i++)
    {
        curr_smbl = str[i];
        if(!is_digit(curr_smbl)) {
            if (curr_smbl == '-' && i == 0) {
                is_negative_num = 1;
                continue;
            }
            *error = 1;
            break;
        }

        num = num * 10 + (curr_smbl - '0');
    }

    if (is_negative_num) {
        num *= -1;
    }

    *value = num;
}

int main(int argc, char **argv)
{
    enum { buf_size = 4096 };
    char buf[buf_size];
    const char few_args_error[] = "Too few arguments\n";
    const char many_args_error[] = "Too many arguments\n";
    const char second_arg_error[] = "The second argument (offset) should be a positive number or zero!\n";
    const char third_arg_error[] = "The third argument (filling string length) is not a positive number!\n";
    char *file_name;
    int fd, offset, filling_str_length;
    int error, blocks_count, remainder;
    char filling_char;

    if (argc < 5) {
        write(1, few_args_error, sizeof(few_args_error));
        return 1;
    }

    if (argc > 5) {
        write(1, many_args_error, sizeof(many_args_error));
        return 2;
    }

    file_name = argv[1];
    convert_str_to_int(argv[2], &error, &offset);
    if(error || (offset < 0)) {
        write(2, second_arg_error, sizeof(second_arg_error));
        return 3;
    }

    convert_str_to_int(argv[3], &error, &filling_str_length);
    if(error || (filling_str_length < 1)) {
        write(2, third_arg_error, sizeof(third_arg_error));
        return 4;
    }

    filling_char = argv[4][0];

    fd = open(file_name, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        perror(file_name);
        return 5;
    }

    fill_str_by_smbl(buf, buf_size, filling_char);
    blocks_count = get_whole_blocks_count(filling_str_length, buf_size);
    remainder = filling_str_length % buf_size;
    lseek64(fd, offset, SEEK_SET);

    for (int i = 0; i < blocks_count; i++)
    {
        write(fd, buf, buf_size);
    }

    if (remainder) {
        write(fd, buf, remainder);
    } 
}