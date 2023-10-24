#include <stdio.h>

int is_letter(int smbl) 
{
    return ('A' <= smbl && smbl <= 'Z') 
            || ('a' <= smbl && smbl <= 'z')
            || (smbl >= '0' && smbl <= '9');
}

int main(int argc, char **argv)
{
    char *first_arg;
    int smbl;
    int prev_smbl = ' ';
    int words_count = 0;

    if (argc == 1) 
    {
        printf("Need 1 argument\n");
        return 1;
    }

    if (argc > 2)
    {
        printf("Too many arguments. Need only one argument\n");
        return 2;
    }

    first_arg = argv[1];
    while ((smbl = *first_arg))
    {
        if (!is_letter(smbl) && is_letter(prev_smbl))
            words_count++;

        prev_smbl = smbl;
        first_arg++;
    }

    if (!is_letter(smbl) && is_letter(prev_smbl))
        words_count++;

    printf("Words count: %d\n", words_count);
    return 0;
}