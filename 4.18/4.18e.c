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

int is_digit(char c)
{
    if ('0' <= c && c <= '9')
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
    tmp->next = NULL;
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

    tmp = malloc(sizeof(char) * (el->len + 1));
    copy_str(tmp, el->str, el->len);
    tmp[el->len - 1] = c;
    tmp[el->len] = 0;
    free(el->str);
    el->str = tmp;
    el->len++;
}

void add_item_to_list(struct item **list, char c, enum adding_mode char_adding_mode)
{
    struct item *last = *list;
    if (!*list) {
        create_new_item_for_list(list, c);
        return;
    }

    while (last->next)
    {
        last = last->next;
    }
    

    if (char_adding_mode == new_word_mode) {
        create_new_item_for_list(&last->next, c);
    } else {
        add_char_to_item_string(last, c);
    } 
}

int search_longest_word_length(struct item *list)
{
    int longest_word_length = 0;
    while (list)
    {
        if (list->len > longest_word_length) {
            longest_word_length = list->len;
        }

        list = list->next;
    }
    return longest_word_length;
};

void print_words_with_size(struct item *list, int size)
{
    while (list)
    {
        if(list->len == size) {
            printf("%s ", list->str);
        }

        list = list->next;
    }
    printf("\n");
}

void free_list(struct item **list)
{
    struct item *del, *tmp = *list;
    while (tmp)
    {
        del = tmp;
        tmp = tmp->next;
        free(del->str);
        free(del);
    }
    *list = NULL;
};

int main() 
{
    int c, longest_word_length;
    struct item *list = NULL;
    enum adding_mode char_adding_mode;

    char_adding_mode = new_word_mode;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') {
            longest_word_length = search_longest_word_length(list);
            print_words_with_size(list, longest_word_length);

            free_list(&list);
            char_adding_mode = new_word_mode;
            continue;
        }
        
        if (is_digit(c)) {
            add_item_to_list(&list, c, char_adding_mode);
            char_adding_mode = new_char_mode;
        } else {
            char_adding_mode = new_word_mode;
        }
    }
}