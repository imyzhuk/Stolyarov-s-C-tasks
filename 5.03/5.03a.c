#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char args_error[] = "Too few arguments\n";
    FILE *main_file, *second_file;
    int c, is_row_for_copying, prev_char = '\n';
    int row_length = 0;
    int third_file_description;

    if (argc < 4) {
        fprintf(stderr, args_error);
        return 1;
    }

    main_file = fopen(argv[1], "r");

    if(!main_file) {
        perror(argv[1]);
        return 2;
    }

    second_file = fopen(argv[2], "w");

    if(!second_file) {
        perror(argv[2]);
        return 3;
    }

    third_file_description = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if(third_file_description == -1) {
        perror(argv[3]);
        return 4;
    }

    while ((c = fgetc(main_file)) != EOF)
    {
        if(prev_char == '\n' && c == ' ') {
            is_row_for_copying = 1;
        }

        if(prev_char == '\n' && c != ' ') {
            is_row_for_copying = 0;
        }

        if (is_row_for_copying) {
            fputc(c, second_file);
        }

        row_length++; 

        if (c == '\n') {
            write(third_file_description, &row_length, sizeof(row_length));
            row_length = 0;
        }

        prev_char = c;        
    }

    if (row_length) {
        write(third_file_description, &row_length, sizeof(row_length)); 
    }
}