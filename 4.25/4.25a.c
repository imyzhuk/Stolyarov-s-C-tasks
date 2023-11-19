#include <ncurses.h>
#include <unistd.h>

int main()
{
    int rows, cols, curr_row, curr_col;
    int is_moving_right = 1;
    int circles_count = 0;
    char smbl = '*';
    enum { delay_duration = 100000 };
    initscr();
    cbreak();
    curs_set(0);
    getmaxyx(stdscr, rows, cols);

    curr_row = rows / 2;
    curr_col = 0;
    move(curr_row, curr_col);
    addch(smbl); 
    refresh();
    usleep(delay_duration);

    while (circles_count != 3)
    {
        move(curr_row, curr_col);
        addch(' ');

        if (is_moving_right && (curr_col < cols)) {
            curr_col++;
        } else if (!is_moving_right && (curr_col >= 0)) {
            curr_col--;
        }

        if (curr_col == cols) {
            is_moving_right = 0;
            curr_col--;
        }

        if (curr_col == -1) {
            is_moving_right = 1;
            curr_col++;
            circles_count++;
        }

        move(curr_row, curr_col);
        addch(smbl);
        refresh();
        usleep(delay_duration);
    }
    
    endwin();
}