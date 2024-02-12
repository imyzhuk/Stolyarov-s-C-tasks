#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int get_str_length(char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

int main(int argc, char **argv)
{
    enum { buf_size = 1024 };
    char buf[buf_size];
    const char args_error[] = "Too few arguments\n";
    const char reading_file_error[] = "Problem with file opening for reading: ";
    const char writing_file_error[] = "Problem with file opening for writing: ";
    const char read_error[] = "Problem within reading\n";
    const char write_error[] = "Problem within writing\n";
    int read_bytes_count, written_bytes_count;
    int read_fd = 0, write_fd = 1, error_fd = 2;

    if (argc >= 2) {
       read_fd = open(argv[1], O_RDONLY);
    }

    if (read_fd == -1) {
        write(error_fd, reading_file_error, sizeof(reading_file_error));
        write(error_fd, argv[1], get_str_length(argv[1]));
        write(error_fd, "\n", 1);
        return 2;
    }

    if (argc == 3) {
        umask(0000);
        write_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    if (write_fd == -1) {
        write(error_fd, writing_file_error, sizeof(writing_file_error));
        write(error_fd, argv[2], get_str_length(argv[2]));
        write(error_fd, "\n", 1);
        return 3;
    }

    if (argc > 3) {
        write(error_fd, args_error, sizeof(args_error));
        return 4;
    }
    
    while ((read_bytes_count = read(read_fd, buf, buf_size)))
    {
        if(read_bytes_count == -1) {
            write(error_fd, read_error, sizeof(read_error));
        }

        written_bytes_count = write(write_fd, buf, read_bytes_count);
        if(written_bytes_count == -1) {
            write(error_fd, write_error, sizeof(write_error));
        }
    }
    
    return 0;
}