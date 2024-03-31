#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <fcntl.h>

enum { 
    max_record_length = 60,
    max_length_of_records_count_string = 10,
    stderr_descriptor = 2
};

int get_str_length(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

void print_file_error(char *file_name)
{
    char file_opening_error[] = "Error with opening file: ";

    write(stderr_descriptor, file_opening_error, sizeof(file_opening_error));
    write(stderr_descriptor, file_name, get_str_length(file_name));
    write(stderr_descriptor, ".\n", 2);
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

int merge_two_records_to_third_list(int src1_fd, int src2_fd, int dest_fd, char *src2_record, int src2_record_inputs_count)
{
    enum {
        ok_exit_code = 0,
        not_found_exit_code = -1
    };
    char src1_buf[max_record_length];
    int record_inputs_count1;

    while (read(src1_fd, src1_buf, max_record_length))
    {
        read(src1_fd, &record_inputs_count1, sizeof(record_inputs_count1));
        if (are_strings_same(src1_buf, src2_record)) {
            long a = lseek64(src1_fd, 0, SEEK_CUR);
            lseek64(dest_fd, a - sizeof(record_inputs_count1), SEEK_SET);
            record_inputs_count1 += src2_record_inputs_count;
            write(dest_fd, &record_inputs_count1, sizeof(record_inputs_count1));
            return ok_exit_code;
        }
    }
    return not_found_exit_code;
}

int main(int argc, char **argv)
{
    enum { max_buf_size = 4096 };
    char buf[max_buf_size];
    char record_buf2[max_record_length];
    const char few_args_error[] = "Too few arguments\n";
    const char many_args_error[] = "Too many arguments\n";
    int source1_fd, source2_fd, dest_file_fd;
    int read_bytes_count;
    int record_inputs_count2;
    int is_merged_record;

    if (argc < 4) {
        write(stderr_descriptor, few_args_error, sizeof(few_args_error));
        return 1;
    }

    if (argc > 4) {
        write(stderr_descriptor, many_args_error, sizeof(many_args_error));
        return 1;
    }

    source1_fd = open(argv[1], O_RDONLY);
    source2_fd = open(argv[2], O_RDONLY);
    dest_file_fd = open(argv[3], O_WRONLY | O_CREAT, 0666);

    if (source1_fd == -1) {
        print_file_error(argv[1]);
        return 2;
    }

    if (source2_fd == -1) {
        print_file_error(argv[2]);
        return 2;
    }

    if (dest_file_fd == -1) {
        print_file_error(argv[3]);
        return 2;
    }

    while ((read_bytes_count = read(source1_fd, buf, max_buf_size)))
    {
        write(dest_file_fd, buf, read_bytes_count);
    }
    lseek64(source1_fd, 0, SEEK_SET);

    while (read(source2_fd, record_buf2, max_record_length))
    {
        read(source2_fd, &record_inputs_count2, sizeof(record_inputs_count2));
        is_merged_record = merge_two_records_to_third_list(source1_fd, source2_fd, dest_file_fd, record_buf2, record_inputs_count2);
        
        if (is_merged_record == -1) {
            lseek64(dest_file_fd, 0, SEEK_END);
            write(dest_file_fd, record_buf2, sizeof(record_buf2));
        }
    }
}