#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;
    int c;
    char *filename = argv[1];

    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments!\n");
        return 1;
    }
    
    file = fopen(filename, "w");
    if (!file) {
        perror(filename);
        return 2;
    }

    while ((c = getchar()) != EOF)
    {
        fputc(c, file);
    }
    
    fclose(file);
}