#include <ncurses.h>

int main()
{
    int rows, cols, curr_row, curr_col;
    int is_moving_right = 1;
    char smbl = '*';
    enum { delay_duration = 100 };
    initscr();
    cbreak();
    curs_set(0);
    keypad(stdscr, 1);
    timeout(delay_duration);
    getmaxyx(stdscr, rows, cols);

    curr_row = rows / 2;
    curr_col = 0;
    move(curr_row, curr_col);
    addch(smbl); 
    refresh();

    while (ERR == getch())
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
        }

        move(curr_row, curr_col);
        addch(smbl);
        refresh();
    }
    
    endwin();
}