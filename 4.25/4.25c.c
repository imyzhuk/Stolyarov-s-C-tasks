#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

enum direction {
    up, 
    down, 
    left, 
    right
};

struct coords {
    int x;
    int y;
    enum direction direction;
};

struct borders {
    int left;
    int right;
    int top;
    int bottom;
};

void fill_scr_part(int smbl, int y, int x, int size)
{
    for (int i = 0; i < size; i++) 
    {
        move(y + i, x);

        for (int j = 0; j < size; j++)
            addch(smbl);
    }
    
    refresh();
}

void draw_square(int smbl, struct coords *rect_coords, int size)
{
    fill_scr_part(smbl, rect_coords->y, rect_coords->x, size);
}

void show_smbl(int smbl, const struct coords *smbl_coords)
{
    move(smbl_coords->y, smbl_coords->x);
    addch(smbl);
    refresh();
}

void hide_smbl(const struct coords *smbl_coords)
{
    show_smbl(' ', smbl_coords);
}

void set_direction(struct coords *smbl_coords, const struct borders *borders)
{
    if (
        smbl_coords->y == borders->top 
        && smbl_coords->x == borders->right
    ) {
        smbl_coords->direction = down;
    } else
    if (
        smbl_coords->y == borders->bottom
        && smbl_coords->x == borders->right
    ) {
        smbl_coords->direction = left;
    } else
    if (
        smbl_coords->y == borders->bottom 
        && smbl_coords->x == borders->left
    ) {
        smbl_coords->direction = up;
    } else
    if (
        smbl_coords->y == borders->top 
        && smbl_coords->x == borders->left
    ) {
        smbl_coords->direction = right;
    }
}

void change_smbl_coords(struct coords *smbl_coords)
{
    switch (smbl_coords->direction) {
    case right:
        ++smbl_coords->x; 
        break;
    case down:
        ++smbl_coords->y; 
        break;
    case left:
        --smbl_coords->x; 
        break;
    case up:
        --smbl_coords->y; 
        break;
    }
}

void move_smbl(int smbl, struct coords *smbl_coords, const struct borders *borders)
{
    hide_smbl(smbl_coords);
    set_direction(smbl_coords, borders);
    change_smbl_coords(smbl_coords);
    show_smbl(smbl, smbl_coords);
}

void clear_scr(const struct borders *filled_scr_part_borders)
{
    fill_scr_part(
        ' ', 
        filled_scr_part_borders->top, 
        filled_scr_part_borders->left,
        filled_scr_part_borders->right - filled_scr_part_borders->left + 1
    );
}

int main()
{
    int cols, rows;
    struct coords rect_coords, moving_smbl_coords;
    struct borders moving_smbl_borders;
    enum { 
        rect_size = 10,
        rect_smbl = '*',
        moving_smbl = '#',
        smbl_size = 1,
        delay_duration = 100,
        delay_after_clear_screen = 300000 
    };

    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, rows, cols);
    timeout(delay_duration);

    if (rows < smbl_size * 2 + rect_size) {
        endwin();
        fprintf(stderr, "Screen must have 12 chars height at least!\n");
        return 1;
    } else
    if (cols < smbl_size * 2 + rect_size) {
        endwin();
        fprintf(stderr, "Screen must have 12 chars width at least!\n");
        return 1;
    }
    

    rect_coords.y = (rows - rect_size) / 2;
    rect_coords.x = (cols - rect_size) / 2;
    draw_square(rect_smbl, &rect_coords, rect_size);

    moving_smbl_borders.top = rect_coords.y - 1;
    moving_smbl_borders.left = rect_coords.x - 1;
    moving_smbl_borders.bottom = moving_smbl_borders.top + rect_size + 1;
    moving_smbl_borders.right = moving_smbl_borders.left + rect_size + 1;

    moving_smbl_coords.y = moving_smbl_borders.top;
    moving_smbl_coords.x = moving_smbl_borders.left;
    moving_smbl_coords.direction = right;
    show_smbl(moving_smbl, &moving_smbl_coords);

    while (ERR == getch())
        move_smbl(moving_smbl, &moving_smbl_coords, &moving_smbl_borders);

    clear_scr(&moving_smbl_borders);
    usleep(delay_after_clear_screen);
    endwin();
}