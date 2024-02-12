#include <stdio.h>
#include <stdlib.h>

struct item
{
    char *str;
    struct item *next;
};

enum adding_mode {
    new_word_mode, 
    new_char_mode
};

int is_divider(char c)
{
    if (c == ' ' || c == '\t')
        return 1;

    return 0;
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

int get_str_length(char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

void add_char_to_item_string(struct item *el, char c)
{
    char *tmp;
    int el_length = get_str_length(el->str);

    tmp = malloc(sizeof(char) * (el_length + 2));
    copy_str(tmp, el->str, el_length);
    tmp[el_length] = c;
    tmp[el_length + 1] = 0;
    free(el->str);
    el->str = tmp;
}

void create_new_item_for_list(struct item **list, char c)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->str = malloc(sizeof(char) * 2);
    tmp->str[0] = c;
    tmp->str[1] = 0;
    tmp->next = NULL;
    *list = tmp;
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

void print_words_in_square_brackets(struct item *list)
{
    while (list)
    {
        printf("[%s]\n", list->str);
        list = list->next;
    }
}


int main() 
{
    int c;
    struct item *list = NULL;
    enum adding_mode char_adding_mode;
    int are_dividers_ignored = 0;
    char quotes_error_message[] = "Error: unmatched quotes\n";

    char_adding_mode = new_word_mode;
    printf("> ");
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') {
            if (are_dividers_ignored) {
                printf("%s", quotes_error_message);
            } else {
                print_words_in_square_brackets(list);
            }

            char_adding_mode = new_word_mode;
            are_dividers_ignored = 0;
            free_list(&list);
            printf("> ");
            continue;
        }

        if (c == '"') {
            are_dividers_ignored = !are_dividers_ignored;
            continue;
        }
        
        if (!are_dividers_ignored && is_divider(c)) {
            char_adding_mode = new_word_mode;
        } else {
            add_item_to_list(&list, c, char_adding_mode);
            char_adding_mode = new_char_mode;
        }
    }

    putchar('\n');
    return 0;
}