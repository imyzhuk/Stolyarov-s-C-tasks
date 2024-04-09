#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t is_smbl_positive = 1;
volatile sig_atomic_t is_pressed_ctrlc = 0;

void write_the_char()
{
    char c = is_smbl_positive ? '+' : '-';
    write(1, &c, 1);
}

void sigint_handler(int s)
{
    signal(SIGINT, sigint_handler);
    is_pressed_ctrlc++;
    if (is_pressed_ctrlc > 1) {
        exit(0);
    }
    is_smbl_positive = 0;
    write_the_char();
    alarm(1);
}

void sigquit_handler(int s)
{
    signal(SIGQUIT, sigquit_handler);
    is_pressed_ctrlc = 0;
    is_smbl_positive = 1;
    write_the_char();
    alarm(1);
}

void sigalrm_handler(int s)
{
    signal(SIGALRM, sigalrm_handler);
    is_pressed_ctrlc = 0;
    write_the_char();
    alarm(1);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    signal(SIGALRM, sigalrm_handler);

    write_the_char();
    alarm(1);

    while (1)
    {
        pause();
    }
}
