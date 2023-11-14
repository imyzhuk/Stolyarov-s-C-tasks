#include <stdio.h>
#include <stdlib.h>

struct file_item {
    char *filename;
    char *longest_string;
    int longest_string_len;
    struct file_item *next;
};

void add_file_item(char *filename, struct file_item **file_list)
{
    struct file_item *tmp, *last = *file_list;
    tmp = malloc(sizeof(struct file_item));
    tmp->filename = filename;
    tmp->longest_string = NULL;
    tmp->longest_string_len = 0;
    tmp->next = NULL;

    if (!*file_list) {
        *file_list = tmp;
        return;
    }

    while (last && last->next)
    {
        last = last->next;
    }
    
    last->next = tmp;
}

void copy_str(char *dest, const char *src, int size)
{
    while (size)
    {
        *dest = *src;
        src++;
        dest++;
        size--;
    }
}

void add_char_to_string(int c, char **str, int str_len)
{
    char *tmp;
    int tmp_len;

    if (!*str) {
        *str = malloc(sizeof(char));
        (*str)[0] = c;
        return;
    }

    tmp_len = str_len + 1;
    tmp = malloc(sizeof(char) * (tmp_len));
    copy_str(tmp, *str, str_len);
    tmp[tmp_len - 1] = c;

    free(*str);
    *str = tmp;
}
void get_file_longest_string(FILE *file, struct file_item *file_item)
{
    int c, prev_char, longest_string_len = 0, current_string_len = 0;
    char *longest_string = NULL, *current_string = NULL;

    while ((c = fgetc(file)) != EOF)
    {
        prev_char = c;
        add_char_to_string(c, &current_string, current_string_len);
        current_string_len++;

        if(c != '\n') {
            continue;
        }

        if (longest_string_len < current_string_len) {
            longest_string_len = current_string_len;
            free(longest_string);
            longest_string = current_string;
            add_char_to_string('\0', &longest_string, longest_string_len);
        }

        current_string = NULL;
        current_string_len = 0;
    }

    if ((prev_char != '\n') && (longest_string_len < current_string_len)) {
            longest_string_len = current_string_len;
            free(longest_string);
            longest_string = current_string;
            add_char_to_string('\n', &longest_string, longest_string_len);
            add_char_to_string('\0', &longest_string, longest_string_len + 1);
    }
    
    file_item->longest_string = longest_string;
    file_item->longest_string_len = longest_string_len;
}

int get_longest_string_size_in_list(struct file_item *list)
{
    int longest_string_size = 0;
    while (list)
    {
        if (longest_string_size < list->longest_string_len) {
            longest_string_size = list->longest_string_len;
        }
        list = list->next;
    }
    return longest_string_size;
}
int main(int argc, char **argv)
{
    FILE *file;
    char *filename;
    int all_files_longest_string_size;
    struct file_item *file_list = NULL;
    struct file_item *last_file_item = NULL;
    
    
    for (int i = 1; i < argc; i++)
    {
        filename = argv[i];
        file = fopen(filename, "r");

        if (!file) {
            perror(filename);
            return 1;
        }
        
        add_file_item(filename, &file_list);

        last_file_item = file_list;
        while (last_file_item && last_file_item->next)
        {
            last_file_item = last_file_item->next;
        }

        get_file_longest_string(file, last_file_item);
    }

    all_files_longest_string_size = get_longest_string_size_in_list(file_list);
    while (file_list)
    {
        if (all_files_longest_string_size == file_list->longest_string_len) {
            putchar('*');
        }
        printf("%s: %s", file_list->filename, file_list->longest_string);
        file_list = file_list->next;
    }
}