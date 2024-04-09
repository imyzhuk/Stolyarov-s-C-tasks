#include <stdlib.h>
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

void push_string_to_strings_array(char ***array, char *str)
{
    char **tmp;
    int array_length;

    if (!*array) {
        tmp = malloc(sizeof(char *) * 2);
        tmp[0] = str;
        tmp[1] = NULL;
        *array = tmp;
        return;
    }

    array_length = get_array_length(*array);
    tmp = malloc(sizeof(char *) * (array_length + 2));
    copy_array(tmp, *array, array_length);
    tmp[array_length] = str;
    tmp[array_length + 1] = NULL;
    
    free(*array);
    *array = tmp;
}