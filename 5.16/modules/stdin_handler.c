#include <stdio.h>
#include <stdlib.h>
#include "stdin_handler.h"
#include "strings_utils.h"

enum adding_mode {
    new_word_mode, 
    new_char_mode
};

static int is_divider(char c)
{
    if (c == ' ' || c == '\t')
        return 1;

    return 0;
}

static void add_char_to_item_string(struct item *el, char c)
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

static void create_new_item_for_list(struct item **list, char c)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->str = malloc(sizeof(char) * 2);
    tmp->str[0] = c;
    tmp->str[1] = 0;
    tmp->next = NULL;
    *list = tmp;
}

static void add_item_to_list(struct item **list, char c, enum adding_mode char_adding_mode)
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

static void free_list(struct item **list)
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

void read_from_stdin(struct item **list, void (*callback)(struct item *))
{
    int c;
    int are_dividers_ignored = 0;
    enum adding_mode char_adding_mode;
    char quotes_error_message[] = "Error: unmatched quotes\n";
    enum {
        initial_char_value = -1,
        impossible_char_value = -2
    };
    signed char prev_char = initial_char_value, prev_prev_char = initial_char_value, prev_prev_prev_char = initial_char_value;

    char_adding_mode = new_word_mode;
    printf("> ");
    while ((c = getchar()) != EOF)
    {        
        if (c == '\n') {
            if (are_dividers_ignored) {
                printf("%s", quotes_error_message);
            } else {
                if (char_adding_mode == new_word_mode && prev_char == '"' && prev_prev_char == '"' && (prev_prev_prev_char == ' ' || prev_prev_prev_char == initial_char_value)) {
                    add_item_to_list(list, '\0', new_word_mode);
                }
                callback(*list);
            }

            char_adding_mode = new_word_mode;
            are_dividers_ignored = 0;
            prev_char = initial_char_value;
            prev_prev_char = initial_char_value;
            prev_prev_prev_char = initial_char_value;
            free_list(list);
            printf("> ");
            continue;
        }
        
        if (!are_dividers_ignored && is_divider(c)) {
            if (char_adding_mode == new_word_mode && prev_char == '"' && prev_prev_char == '"' && (prev_prev_prev_char == ' ' || prev_prev_prev_char == initial_char_value)) {
                add_item_to_list(list, '\0', new_word_mode);
            }
            char_adding_mode = new_word_mode;
        } else if (c == '"' && prev_char != '\\') {
            are_dividers_ignored = !are_dividers_ignored;
        } else if (c == '\\' && prev_char != '\\') {
            //nothing
        } else if (c == '\\' && prev_char == '\\') {
            add_item_to_list(list, c, char_adding_mode);
            char_adding_mode = new_char_mode;
            prev_char = impossible_char_value;
            continue;
        } else {
            add_item_to_list(list, c, char_adding_mode);
            char_adding_mode = new_char_mode;
        }
        
        prev_prev_prev_char = prev_prev_char;
        prev_prev_char = prev_char;
        prev_char = c;
    }

    putchar('\n');
}