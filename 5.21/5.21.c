#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_calls_count = 0;
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
    signal(SIGINT, sigint_handler);
    sigint_calls_count++;
}

void sigusr1_handler(int s)
{
    char sigint_calls_count_string[max_length_of_string];
    int sigint_calls_count_string_len;
    signal(SIGUSR1, sigusr1_handler);

    convert_num_to_string(sigint_calls_count, sigint_calls_count_string, &sigint_calls_count_string_len);
    write(1, sigint_calls_count_string, sigint_calls_count_string_len);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    while (1)
    {
        pause();
    }

    return 0;
}
