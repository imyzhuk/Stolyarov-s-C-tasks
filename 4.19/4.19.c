#include <stdio.h>
#include <stdlib.h>

struct item
{
    long value;
    struct item *next;
};

void create_new_item_for_list(struct item **list, long num)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->value = num;
    tmp->next = NULL;
    *list = tmp;
}

void add_item_to_list(struct item **list, long num)
{
    struct item *last = *list;
    if (!*list) {
        create_new_item_for_list(list, num);
        return;
    }

    while (last->next)
    {
        last = last->next;
    }
    
    create_new_item_for_list(&last->next, num);
}

int main() 
{
    long number, digits_count, c;
    struct item *list = NULL;
    while ((c = getchar()) != EOF)
    {
        if (c == '-' || (c >= '0' && c <= '9')) {
            ungetc(c, stdin);
            digits_count = scanf("%ld", &number);
            if (digits_count) {
                add_item_to_list(&list, number);
            }
        }
    }

    while (list && list->next)
    {
        if (
            (list->value + 5 >= list->next->value)
            && (list->value - 5 <= list->next->value)
         ) {
            printf("%ld %ld\n", list->value, list->next->value);
        }
        list = list->next;
    }     
}