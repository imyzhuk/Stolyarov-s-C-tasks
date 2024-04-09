#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t is_signint_got = 0;
char initial_str[] = "Press Ctrl-C to quit\n";
char finished_str[] = "Good bye\n";

void handler(int s)
{
    is_signint_got = 0;
}

int main(int argc, char const *argv[])
{
    sigset_t mask_int, mask_empty;
    sigemptyset(&mask_int);
    sigaddset(&mask_int, SIGINT);
    sigemptyset(&mask_empty);

    sigprocmask(SIG_SETMASK, &mask_int, NULL);
    signal(SIGINT, handler);
    write(1, initial_str, sizeof(initial_str));

    while (!is_signint_got)
    {
        sigsuspend(&mask_empty);
    }

    write(1, finished_str, sizeof(finished_str));
}
