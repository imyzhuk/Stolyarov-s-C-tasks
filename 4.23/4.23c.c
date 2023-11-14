#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;
    int c, prev_char;
    char *filename = argv[1];
    int strings_count = 0;

    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments!\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return 2;
    }

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
            strings_count++;

        prev_char = c;
    }

    if (prev_char != '\n')
        strings_count++;
    

    fclose(file);

    printf("%d\n", strings_count);
}