#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t is_pressed_ctrlc = 0;
volatile sig_atomic_t letters_count = 0;
volatile sig_atomic_t rows_count = 0;
char sleep_msg[] = "Are you sleeping?\n";
char letters_count_msg[] = "Letters count: ";
char rows_count_msg[] = "rows count: ";
enum { 
    max_length_of_string = 10 
};

void convert_num_to_string(sig_atomic_t num, char *str, int *len)
{
    int str_len = 0;
    int digit;
    sig_atomic_t tmp = num;
    do {
        tmp /= 10;
        str_len++;
    } while (tmp);

    str[str_len] = '\0';
    for (int i = 0; i < str_len; i++)
    {
        digit = num % 10;
        num /= 10;
        str[str_len - 1 - i] = digit + '0';
    }

    *len = str_len;    
}

void sigint_handler(int s)
{
    char letters_count_string[max_length_of_string];
    char rows_count_string[max_length_of_string];
    int letters_count_string_len, rows_count_string_len;
    signal(SIGINT, sigint_handler);
    is_pressed_ctrlc++;
    if (is_pressed_ctrlc > 1) {
        exit(0);
    }

    convert_num_to_string(letters_count, letters_count_string, &letters_count_string_len);
    convert_num_to_string(rows_count, rows_count_string, &rows_count_string_len);
    write(1, letters_count_msg, sizeof(letters_count_msg));
    write(1, letters_count_string, letters_count_string_len);
    write(1, ", ", 2);
    write(1, rows_count_msg, sizeof(rows_count_msg));
    write(1, rows_count_string, rows_count_string_len);
    write(1, "\n", 1);

    alarm(5);
}


void sigalrm_handler(int s)
{
    signal(SIGALRM, sigalrm_handler);
    is_pressed_ctrlc = 0;
    write(1, sleep_msg, sizeof(sleep_msg));
}

int main(int argc, char const *argv[])
{
    enum { buf_size = 1 };
    char buf[buf_size];

    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);

    alarm(5);
    while (read(0, buf, buf_size))
    {
        is_pressed_ctrlc = 0;
        alarm(5);
        if (buf[0] == '\n') {
            rows_count++;
        } else {
            letters_count++;
        }
    }
    
}
