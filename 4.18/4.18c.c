#include <stdio.h>
#include <stdlib.h>

struct item
{
    char *str;
    int len;
    struct item *next;
};

enum adding_mode {
    new_word_mode, 
    new_char_mode
};

int is_letter(char c)
{
    if (
        ('A' <= c && c <= 'Z') 
        || ('a' <= c && c <= 'z')
    )
        return 1;

    return 0;
}

void create_new_item_for_list(struct item **list, char c)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->str = malloc(sizeof(char) * 2);
    tmp->str[0] = c;
    tmp->str[1] = 0;
    tmp->len = 2;
    tmp->next = *list;
    *list = tmp;
}

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
void add_char_to_item_string(struct item *el, char c)
{
    char *tmp;

    el->len++;
    tmp = malloc(sizeof(char) * el->len);
    copy_str(tmp, el->str, el->len - 1);
    tmp[el->len - 2] = c;
    tmp[el->len - 1] = 0;
    free(el->str);
    el->str = tmp;
}

void add_item_to_list(struct item **list, char c, enum adding_mode char_adding_mode)
{
    if (!*list) {
        create_new_item_for_list(list, c);
        return;
    }

    if (char_adding_mode == new_word_mode) {
        create_new_item_for_list(list, c);
    } else {
        add_char_to_item_string(*list, c);
    } 
}

int main() 
{
    int c;
    struct item *list = NULL;
    enum adding_mode char_adding_mode;

    char_adding_mode = new_word_mode;
    while ((c = getchar()) != EOF)
    {
        if (is_letter(c)) {
            add_item_to_list(&list, c, char_adding_mode);
            char_adding_mode = new_char_mode;
        } else {
            char_adding_mode = new_word_mode;
        }
    }

    while (list)
    {
        printf("%s ", list->str);
        list = list->next;
    }
    printf("\n"); 
}