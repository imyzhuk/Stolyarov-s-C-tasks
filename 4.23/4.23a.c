#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;
    char *filename = argv[1];
    char *text = 
    "Humpty Dumpty sat on a wall.\n"
    "Humpty Dumpty had a great fall.\n"
    "All the king's horses and all the king's men\n"
    "couldn't put Humpty together again.\n\n"

    "Humpty Dumpty sat on a wall.\n"
    "Humpty Dumpty had a great fall.\n"
    "All the king's horses and all the king's men\n"
    "couldn't put Humpty together again.\n\n"

    "Humpty Dumpty sat on a wall.\n"
    "Humpty Dumpty had a great fall.\n"
    "All the king's horses and all the king's men\n"
    "couldn't put Humpty together again.\n"; 

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

    fprintf(file, "%s", text);

    fclose(file);
}