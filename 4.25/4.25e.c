#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

struct rect {
    int y;
    int x;
    int height;
    int width;
    int max_height;
    int max_width;
};

enum changing_action {
    increase,
    decrease
};

enum changing_direction {
    horizontal,
    vertical
};

enum { rect_smbl = '*' };

void draw_vertical_line(int smbl, int y, int x, int height)
{
    for (int i = 0; i < height; i++)
    {
        move(y + i, x);
        addch(smbl);
    }
}

void draw_horizontal_line(int smbl, int y, int x, int width)
{
    move(y, x);

    for (int i = 0; i < width; i++)
        addch(smbl);
}

void fill_scr_part(int smbl, int y, int x, int height, int width)
{
    for (int i = 0; i < height; i++) 
    {
        move(y + i, x);
        draw_horizontal_line(smbl, y + i, x, width);
    }

    refresh();
}

void draw_rect(const struct rect *rect, int smbl)
{
    fill_scr_part(
        smbl, 
        rect->y, 
        rect->x, 
        rect->height, 
        rect->width
    );
}

void check_rect_size(struct rect *rect)
{
    if(rect->height > rect->max_height) {
        rect->height -= 2;
        rect->y++;
    } else
    if(rect->width > rect->max_width) {
        rect->width -= 2;
        rect->x++;
    } else
    if(rect->height < 1) {
        rect->height = 1;
        rect->y--;
    } else
    if(rect->width < 1) {
        rect->width = 1;
        rect->x--;
    }
}
void change_rect_size(
    struct rect *rect, 
    enum changing_direction direction,
    enum changing_action action
)
{
    int init_x, init_y;
    init_y = rect->y;
    init_x = rect->x;

    if(direction == horizontal && action == increase) {
        rect->width += 2;
        rect->x -= 1;
    } else
    if(direction == horizontal && action == decrease) {
        rect->width -= 2;
        rect->x += 1;
    } else
    if(direction == vertical && action == increase) {
        rect->height += 2;
        rect->y -= 1;
    } else
    if(direction == vertical && action == decrease) {
        rect->height -= 2;
        rect->y += 1;
    }

    check_rect_size(rect);

    if (rect->x < init_x) {
        draw_vertical_line(rect_smbl, rect->y, rect->x, rect->height);
        draw_vertical_line(rect_smbl, rect->y, rect->x + rect->width - 1, rect->height);
    } else
    if (rect->x > init_x) {
        draw_vertical_line(' ', rect->y, init_x, rect->height);
        draw_vertical_line(' ', rect->y, rect->x + rect->width, rect->height);
    } else
    if (rect->y < init_y) {
        draw_horizontal_line(rect_smbl, rect->y, rect->x, rect->width);
        draw_horizontal_line(rect_smbl, rect->y + rect->height - 1, rect->x, rect->width);
    } else
    if (rect->y > init_y) {
        draw_horizontal_line(' ', init_y, rect->x, rect->width);
        draw_horizontal_line(' ', rect->y + rect->height, rect->x, rect->width);
    }
    
}

void clear_scr(int y, int x, int height, int width)
{
    fill_scr_part(' ', y, x, height, width);
}

int main()
{
    int rows, cols, key;
    struct rect rect;
    enum { 
        key_escape = 27,
        key_space = ' ',
        init_rect_size = 3,
        delay_after_clear_screen = 300000
    };

    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, rows, cols);

    rect.y = (rows - init_rect_size) / 2;
    rect.x = (cols - init_rect_size) / 2;
    rect.height = init_rect_size;
    rect.width = init_rect_size;
    rect.max_height = rows / 2;
    rect.max_width = cols / 2;
    draw_rect(&rect, rect_smbl);

    while ((key = getch()))
    {
        if(key == key_escape || key == key_space) 
            break;

        switch (key) {
        case KEY_UP:
            change_rect_size(&rect, vertical, increase);
            break;
        case KEY_DOWN:
            change_rect_size(&rect, vertical, decrease);
            break;
        case KEY_LEFT:
            change_rect_size(&rect, horizontal, decrease);
            break;
        case KEY_RIGHT:
            change_rect_size(&rect, horizontal, increase);
            break;
        }
    }

    clear_scr(rect.y, rect.x, rect.height, rect.width);
    usleep(delay_after_clear_screen);
    endwin();
}