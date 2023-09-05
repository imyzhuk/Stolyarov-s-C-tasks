#include <stdio.h>

int main(int argc, char **argv) {
    int is_there_word_to_print = 0;

    for (int i = 1; i < argc; i++) {
        char *str = argv[i]; 
        if(str[0] != '-') {
            is_there_word_to_print = 1;
            printf("%s ", str); 
        }
    }
    
    if(is_there_word_to_print) putchar('\n');
}