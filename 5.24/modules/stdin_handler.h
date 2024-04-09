#ifndef STDIN_HANDLER_H_SENTRY
#define STDIN_HANDLER_H_SENTRY
struct item
{
    char *str;
    struct item *next;
};

void read_from_stdin(struct item **list, void (*callback)(struct item *));
#endif