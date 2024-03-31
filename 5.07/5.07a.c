#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <fcntl.h>

enum { 
    max_record_length = 60,
    max_length_of_records_count_string = 10 
};

void convert_int_to_string(int num, char *str, int *len)
{
    int str_len = 0;
    int digit;
    int tmp = num;
    do {
        tmp /= 10;
        str_len++;
    } while (tmp);

    str[str_len] = '\0';
    for (int i = 0; i < str_len; i++)
    {
        digit = num % 10;
        num /= 10;
        str[str_len - 1 - i] = digit + '0';
    }

    *len = str_len;    
}

int get_str_length(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

int are_strings_same(char *first_str, char *second_str)
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

void add_record_to_list(char *file_name, char *record)
{
    char record_buf[max_record_length];
    char file_opening_error[] = "Error with opening file: ";
    int record_len;
    int record_inputs_count;
    int fd = open(file_name, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        write(2, file_opening_error, sizeof(file_opening_error));
        write(2, file_name, get_str_length(file_name));
        write(2, ".\n", 2);
        return;
    }

    while (read(fd, record_buf, max_record_length))
    {
        read(fd, &record_inputs_count, sizeof(record_inputs_count));
        if (are_strings_same(record, record_buf)) {
            record_inputs_count += 1;
            lseek64(fd, sizeof(record_inputs_count) * -1, SEEK_CUR);
            write(fd, &record_inputs_count, sizeof(record_inputs_count));
            return;
        }
    }

    record_inputs_count = 1;
    record_len = get_str_length(record);
    write(fd, record, record_len);
    lseek64(fd, max_record_length - record_len, SEEK_CUR);
    write(fd, &record_inputs_count, sizeof(record_inputs_count));
}

void get_records_count_from_list(char *file_name, char *record, int *count)
{
    enum { record_is_not_found = 0 };
    char record_buf[max_record_length];
    char file_opening_error[] = "Error with opening file: ";
    int records_count;
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        write(2, file_opening_error, sizeof(file_opening_error));
        write(2, file_name, get_str_length(file_name));
        write(2, ".\n", 2);
        return;
    }

    while (read(fd, record_buf, max_record_length))
    {
        read(fd, &records_count, sizeof(records_count));
        if (are_strings_same(record, record_buf)) {
            *count = records_count;
            return;
        }
    }

    *count = 0;
}

void print_list_from_file(char *file_name)
{
    char record_buf[max_record_length];
    char file_opening_error[] = "Error with opening file: ";
    int records_count, record_len, records_count_string_len;
    int fd;
    char records_count_string[max_length_of_records_count_string];
    
    fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        write(2, file_opening_error, sizeof(file_opening_error));
        write(2, file_name, get_str_length(file_name));
        write(2, ".\n", 2);
        return;
    }

    while (read(fd, record_buf, max_record_length))
    {
        record_len = get_str_length(record_buf);

        read(fd, &records_count, sizeof(records_count));
        convert_int_to_string(records_count, records_count_string, &records_count_string_len);

        write(1, record_buf, record_len);
        write(1, " ", 1);
        write(1, records_count_string, records_count_string_len);
        write(1, "\n", 1);
    }
}

int main(int argc, char **argv)
{
    const char few_args_error[] = "Too few arguments\n";
    const char many_args_error[] = "Too many arguments\n";
    const char wrong_command_error[] = "You have inputed a wrong command\n";
    const char input_word_is_too_long_error[] = "The input word is too long\n";
    char *file_name, *command;
    int records_count;
    char records_count_string[max_length_of_records_count_string];
    int records_count_string_len;

    if (argc < 3 || (argc == 3 && !are_strings_same(argv[2], "list"))) {
        write(2, few_args_error, sizeof(few_args_error));
        return 1;
    }

    if (argc > 4 || (argc > 3 && are_strings_same(argv[2], "list"))) {
        write(2, many_args_error, sizeof(many_args_error));
        return 2;
    }

    file_name = argv[1];
    command = argv[2];
    

    if (are_strings_same(command, "add")) {
        if (get_str_length(argv[3]) >= max_record_length) {
            write(2, input_word_is_too_long_error, sizeof(input_word_is_too_long_error));
            return 3; 
        }
        
        add_record_to_list(file_name, argv[3]);
    } else if (are_strings_same(command, "query")) {
        get_records_count_from_list(file_name, argv[3], &records_count);
        convert_int_to_string(records_count, records_count_string, &records_count_string_len);
        write(1, records_count_string, records_count_string_len);
        write(2, "\n", 2);
    } else if (are_strings_same(command, "list")) {
        print_list_from_file(file_name);
    } else {
        write(2, wrong_command_error, sizeof(wrong_command_error));
        return 4;
    }

}