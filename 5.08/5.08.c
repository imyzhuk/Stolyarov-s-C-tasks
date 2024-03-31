#include <dirent.h>
#include <stdio.h>
#include <malloc.h>

int get_str_length(const char *str)
{
    int len = 0;

    while (str[len])
        len++;

    return len;
}

int are_strings_same(const char *first_str, const char *second_str)
{
    enum {
        strings_are_not_same = 0,
        strings_are_same = 1
    };
    int i;

    for (i = 0; first_str[i] != '\0' || second_str[i] != '\0'; i++)
    {
        if (first_str[i] != second_str[i]) {
            return strings_are_not_same;
        }
    }

    if (first_str[i] == '\0' && second_str[i] == '\0') {
        return strings_are_same;
    }

    return strings_are_not_same;
}

void join_strings_with_slash(const char *str1, const char *str2, char **result)
{
    int str1_len = get_str_length(str1);
    int str2_len = get_str_length(str2);
    char *buf = malloc(str1_len + str2_len + 2);
    sprintf(buf, "%s/%s", str1, str2);
    *result = buf;
}

void search_file_in_directory(const char *dir_name, const char *file_name)
{
    DIR *dir;
    struct dirent *dent;
    char *new_dir_path;

    dir = opendir(dir_name);
    if (!dir) {
        perror(file_name);
        return;
    }

    
    while ((dent = readdir(dir)) != NULL)
    {
        if (are_strings_same(dent->d_name, ".") || are_strings_same(dent->d_name, "..")) {
            continue;
        }

        join_strings_with_slash(dir_name, dent->d_name, &new_dir_path);
        
        if(opendir(new_dir_path)) {
            search_file_in_directory(new_dir_path, file_name);
            continue;
        }

        if (are_strings_same(file_name, dent->d_name)) {
            printf("%s/%s\n", dir_name, dent->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char const *argv[])
{
    const char wrong_count_of_args_error[] = "Wrong count of arguments!\n";
    const char dir_name[] = ".";

    if (argc != 2) {
        fprintf(stderr, "%s", wrong_count_of_args_error);
        return 1;
    }
    
    search_file_in_directory(dir_name, argv[1]);
}