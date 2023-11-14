#include <stdio.h>
#include <stdlib.h>

struct item
{
    long value;
    int times;
    struct item *next;
};

void create_new_item_for_list(struct item **list, long num)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->value = num;
    tmp->times = 1;
    tmp->next = NULL;
    *list = tmp;
}

void add_item_to_list(struct item **list, long num)
{
    struct item *last, *tmp = *list;
    if (!*list) {
        create_new_item_for_list(list, num);
        return;
    }

    while (tmp)
    {
        if (tmp->value == num)
        {
            tmp->times++;
            return;
        }
        last = tmp;
        tmp = tmp->next;
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

    while (list)
    {
        if (list->times == 3) {
            printf("%ld\n", list->value);
        }
        list = list->next;
    }     
}