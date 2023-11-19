#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

enum direction {
    up, 
    down, 
    left, 
    right
};

struct coords {
    int x;
    int y;
};

struct settings {
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

void show_smbl(int smbl, const struct settings *smbl_settings)
{
    move(smbl_settings->y, smbl_settings->x);
    addch(smbl);
    refresh();
}

void hide_smbl(const struct settings *smbl_settings)
{
    show_smbl(' ', smbl_settings);
}

void check_direction(struct settings *smbl_settings, const struct borders *borders)
{
    if (
        smbl_settings->y == borders->top 
        && smbl_settings->x == borders->right
        && smbl_settings->direction == right
    ) {
        smbl_settings->direction = down;
    } else
    if (
        smbl_settings->y == borders->bottom
        && smbl_settings->x == borders->right
        && smbl_settings->direction == down
    ) {
        smbl_settings->direction = left;
    } else
    if (
        smbl_settings->y == borders->bottom 
        && smbl_settings->x == borders->left
        && smbl_settings->direction == left
    ) {
        smbl_settings->direction = up;
    } else
    if (
        smbl_settings->y == borders->top 
        && smbl_settings->x == borders->left
        && smbl_settings->direction == up
    ) {
        smbl_settings->direction = right;
    } else
    if (
        smbl_settings->y == borders->top 
        && smbl_settings->x == borders->left
        && smbl_settings->direction == left
    ) {
        smbl_settings->direction = down;
    } else
    if (
        smbl_settings->y == borders->bottom
        && smbl_settings->x == borders->left
        && smbl_settings->direction == down
    ) {
        smbl_settings->direction = right;
    } else
    if (
        smbl_settings->y == borders->bottom 
        && smbl_settings->x == borders->right
        && smbl_settings->direction == right
    ) {
        smbl_settings->direction = up;
    } else
    if (
        smbl_settings->y == borders->top 
        && smbl_settings->x == borders->right
        && smbl_settings->direction == up
    ) {
        smbl_settings->direction = left;
    }
}

void change_smbl_settings(struct settings *smbl_settings)
{
    switch (smbl_settings->direction) {
    case right:
        ++smbl_settings->x; 
        break;
    case down:
        ++smbl_settings->y; 
        break;
    case left:
        --smbl_settings->x; 
        break;
    case up:
        --smbl_settings->y; 
        break;
    }
}

void move_smbl(int smbl, struct settings *smbl_settings, const struct borders *borders)
{
    hide_smbl(smbl_settings);
    check_direction(smbl_settings, borders);
    change_smbl_settings(smbl_settings);
    show_smbl(smbl, smbl_settings);
}

void change_direction(struct settings *smbl_settings)
{
    switch (smbl_settings->direction) {
    case right:
        smbl_settings->direction = left; 
        break;
    case down:
        smbl_settings->direction = up; 
        break;
    case left:
        smbl_settings->direction = right; 
        break;
    case up:
        smbl_settings->direction = down; 
        break;
    }
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
    int cols, rows, key, delay_duration;
    struct coords rect_coords; 
    struct settings moving_smbl_settings;
    struct borders moving_smbl_borders;
    enum { 
        rect_size = 10,
        rect_smbl = '*',
        moving_smbl = '#',
        smbl_size = 1,
        init_delay_duration = 100,
        delay_after_clear_screen = 300000,
        key_escape = 27
    };

    delay_duration = init_delay_duration;

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

    moving_smbl_settings.y = moving_smbl_borders.top;
    moving_smbl_settings.x = moving_smbl_borders.left;
    moving_smbl_settings.direction = right;
    show_smbl(moving_smbl, &moving_smbl_settings);

    while ((key = getch()) != key_escape)
    {
        switch (key) {
        case ' ':
            change_direction(&moving_smbl_settings);
            break;
        case KEY_LEFT:
            delay_duration *= 2;
            timeout(delay_duration);
            break;
        case KEY_RIGHT:
            delay_duration /= 2;
            timeout(delay_duration);
            break;
        }

        move_smbl(moving_smbl, &moving_smbl_settings, &moving_smbl_borders);
    }

    clear_scr(&moving_smbl_borders);
    usleep(delay_after_clear_screen);
    endwin();
}