#include <stdio.h>

int main(int argc, char **argv) {
    enum { buf_size = 10 };
    FILE *from_file;
    char *filename = argv[1];
    char buf[buf_size];

    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments!\n");
        return 1;
    }
    
    from_file = fopen(filename, "r");
    if (!from_file) {
        perror(filename);
        return 2;
    }

    while (fgets(buf, buf_size, from_file))
    {
        printf("%s", buf);
    }
    
    fclose(from_file);
}