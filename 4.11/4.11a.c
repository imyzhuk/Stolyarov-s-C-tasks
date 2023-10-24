#include <stdio.h>

int str_len(const char *str)
{
    const char *i = str;
    while (*i)
        i++;
    return i - str;
}

int check_word_by_latin_letter(const char *str)
{
    const char *i = str;
    while (*i)
    {
        int current_smbl = *i;
        if (
            ('A' <= current_smbl && current_smbl <= 'Z') 
            || ('a' <= current_smbl && current_smbl <= 'z')
        )
            return 1;
        i++;   
    }

    return 0;
}

int check_word_by_only_digits(const char *str)
{
    const char *i = str;
    while (*i)
    {
        int current_smbl = *i;
        if (current_smbl < '0' || current_smbl > '9')
            return 0;
        i++;   
    }

    return 1;
}

/* return 1 if there are no duplicates*/
int check_word_by_unique_smbls(const char *str)
{
    if (!*str) return 1;

    const char *p = str;
    int first_letter = p[0];
    p++;
    while (*p)
    {
        if (first_letter == *p)
            return 0;
        p++;
    }

    return check_word_by_unique_smbls(str + 1);
}

/* return 1 if all symbols are same or there is only one smbl in string*/
int check_word_by_same_smbl(const char *str) 
{
    const char *p = str;
    int first_letter = p[0];
    p++;
    while (*p)
    {
        if (first_letter != *p)
            return 0;
        p++;
    }
    return 1;
}

int check_word_by_dot_and_smbl_AT(const char *str) {
    const char *p = str;
    int dots_count = 0;
    int AT_smbl_count = 0;
    while (*p) {
        if (*p == '.') dots_count++;
        if (*p == '@') AT_smbl_count++;
        p++;
    }
    if(dots_count && (AT_smbl_count == 1)) return 1;
    else return 0;
}

int check_word_by_same_smbl_presence(const char *src_word, const char *checked_word)
{
    if (!*src_word) return 0;

    int first_letter = src_word[0];
    const char *p = checked_word;
    while (*p)
    {
        if (first_letter == *p)
            return 1;
        p++;
    }

    return check_word_by_same_smbl_presence(src_word + 1, checked_word);
}

void print_words_from(char **str, int arr_len, int (*match) (const char *))
{
    int matches_count = 0;
    for (int i = 0; i < arr_len; i++)
    {
        int is_there_matches = match(str[i]);
        if(is_there_matches) {
            printf("%s ", str[i]);
            matches_count++;
        }
    }

    if(!matches_count) printf("none");
    putchar('\n');
}
int main(int argc, char **argv)
{
    int longest_word_index = 0;
    int longest_word_size = 0;
    int current_word_size;
    int matches_count = 0;

    if(argc == 1) {
        printf("Add arguments!\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        current_word_size = str_len(argv[i]);
        if (current_word_size > longest_word_size)
        {
            longest_word_size = current_word_size;
            longest_word_index = i;
        }       
    }
    printf("Longest arg: %s (length: %d)\n", argv[longest_word_index], longest_word_size);

    printf("Words without duplicates: ");
    print_words_from(argv + 1, argc - 1, check_word_by_unique_smbls);

    printf("Words with one @ and a dot: ");
    print_words_from(argv + 1, argc - 1, check_word_by_dot_and_smbl_AT);

    printf("Words contains only digits: ");
    print_words_from(argv + 1, argc - 1, check_word_by_only_digits);

    printf("Words created by only one symbol: ");
    print_words_from(argv + 1, argc - 1, check_word_by_same_smbl);

    printf("Words contains one latin letter or more: ");
    print_words_from(argv + 1, argc - 1, check_word_by_latin_letter);

    printf("Words contains one common symbol with first arg: ");
    for (int i = 2; i < argc; i++)
    {
        int is_there_matches = check_word_by_same_smbl_presence(argv[1], argv[i]);
        if(is_there_matches) {
            printf("%s ", argv[i]);
            matches_count++;
        }
    }
    if(!matches_count) printf("none");
    putchar('\n');
}