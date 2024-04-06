#include "strings_utils.h"

void copy_str(char *dest, const char *src, int len)
{
    while (len)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    }
    
}

int get_str_length(char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

int are_strings_same(const char *first_str, const char *second_str)
{
    enum {
        strings_are_not_same = 0,
        strings_are_same = 1
    };
    int i;

    for (i = 0; first_str[i] != '\0' || second_str[i] != '\0'; i++)
    {
        if (first_str[i] != second_str[i]) {
            return strings_are_not_same;
        }
    }

    if (first_str[i] == '\0' && second_str[i] == '\0') {
        return strings_are_same;
    }

    return strings_are_not_same;
}

int get_array_length(char **array)
{
    int len = 0;

    while (array[len])
        len++;

    return len;
}

void copy_array(char **dest, char **src, int len)
{
    while (len)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    }
    
}