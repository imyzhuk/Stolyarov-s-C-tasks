#include <stddef.h>
#include <stdio.h>

int str_len(const char *str)
{
    const char *i = str;
    while (*i)
        i++;
    return i - str;
}

const char *find_substring(const char *str, const char *substr)
{
    const char *p = str, *t = substr;
    int substr_len;
    while (*t)
    {
        if (!*p) {
            return NULL;
        }

        if (*p == *t) {
            t++;
        } else {
            t = substr;
        }   
        p++;
    }

    substr_len = str_len(substr);
    return p - substr_len;
}

int find_substr_count_in(const char *str, const char *substr) 
{
    int substr_len;
    const char *substr_address_in_str = find_substring(str, substr);
    if (!substr_address_in_str)
        return 0;

    substr_len = str_len(substr);
    
    return 1 + find_substr_count_in(substr_address_in_str + substr_len, substr);
}

int main(int argc, char **argv) 
{
    if (argc < 3) {
        fprintf(stderr, "Must be more than one argument in a command line\n");
        return 1;
    }

    for (int i = 2; i < argc; i++)
    {
        int substrings_count_in_cmd_arg = find_substr_count_in(argv[i], argv[1]);
        if (substrings_count_in_cmd_arg)
        {
            printf("%s (%d)\n", argv[i], substrings_count_in_cmd_arg);
        }
    }
}