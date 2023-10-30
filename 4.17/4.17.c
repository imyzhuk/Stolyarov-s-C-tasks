#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

struct item {
    int x;
    struct item *next;
};


struct item *int_array_to_list(const int *arr, int len)
{
    struct item *first = NULL, *last = NULL, *tmp;
    for (int i = 0; i < len; i++)
    {
        tmp = malloc(sizeof(struct item));
        tmp->x = arr[i];
        tmp->next = NULL;
        if (last) {
            last->next = tmp;
            last = last->next;
        } else {
            first = last = tmp;
        }
    }
    return first;
}

void copy_arr(int *dest, int *src, int len)
{
    while (len)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    }
}

int *list_to_int_array(struct item *list)
{
    int *arr, *tmp;
    int arr_len = 1;
    arr = malloc(sizeof(int));
    arr[0] = 1;
    while (list)
    {
        arr_len++;
        tmp = malloc(sizeof(int) * arr_len);
        copy_arr(tmp, arr, arr_len - 1);
        tmp[0]++;
        tmp[arr_len - 1] = list->x;

        free(arr);
        arr = tmp;

        list = list->next;
    }
    return arr;
}

int main() 
{
    enum { test_arr_len = 5 };
    int test_arr[] = { 2, 4, 6, 8, 10 };
    int *new_array;
    struct item *list = int_array_to_list(test_arr, test_arr_len);
    new_array = list_to_int_array(list);
    if (test_arr_len + 1 == new_array[0]) {
        printf("OK\n");
    } else {
        printf("No OK\n");
    } 
}