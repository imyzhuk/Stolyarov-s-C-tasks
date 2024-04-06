#ifndef STRINGS_UTILS_H_SENTRY
#define STRINGS_UTILS_H_SENTRY
void copy_str(char *dest, const char *src, int len);

int get_str_length(char *str);

int are_strings_same(const char *first_str, const char *second_str);

int get_array_length(char **array);

void copy_array(char **dest, char **src, int len);
#endif