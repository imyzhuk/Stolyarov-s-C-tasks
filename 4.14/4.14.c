#include <stdio.h>

void delete_spaces_in(char *str)
{
    char *ptr = str;
    while (*str)
    {
        if (*str != ' ')
        {
            *ptr = *str;
            ptr++;
        }
        
        str++;
    }
    *ptr = 0;
    
}

int main() 
{
    char test_str[] = "  He ll    o, w  or     l d!\n";
    delete_spaces_in(test_str);
    printf("%s", test_str);
    return 0;
}