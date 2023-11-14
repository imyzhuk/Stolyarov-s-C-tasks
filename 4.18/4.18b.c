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

int search_max_count_of_repeats(struct item *list)
{
    int max_count_of_repeats = 0;
    struct item *tmp = list;

    while (tmp)
    {
        if (tmp->times > max_count_of_repeats) {
            max_count_of_repeats = tmp->times;
        }

        tmp = tmp->next;
    }

    return max_count_of_repeats;
}

int main() 
{
    long number, digits_count, c;
    struct item *list = NULL;
    int max_count_of_repeats;
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

    max_count_of_repeats = search_max_count_of_repeats(list);

    while (list)
    {
        if (list->times == max_count_of_repeats) {
            printf("%ld\n", list->value);
        }
        list = list->next;
    }     
}