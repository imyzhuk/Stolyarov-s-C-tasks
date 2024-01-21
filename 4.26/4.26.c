#include <ncurses.h>
#include <unistd.h>

struct square {
    int y;
    int x;
    int size;
};

struct color_pair {
    int fg_color_num;
    int bg_color_num;
};

enum { colors_count = 8 };

enum changing_color_direction {
    forward, backward
};

const int colors[colors_count] = {
    COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, 
    COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};

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

void draw_square(const struct square *square, int smbl)
{
    fill_scr_part(
        smbl, 
        square->y, 
        square->x, 
        square->size, 
        square->size 
    );
}

void check_color(int *color_num)
{
    if (*color_num == colors_count)
        *color_num = 0;
    else
    if (*color_num < 0)
        *color_num = colors_count - 1;
}

void change_color(
    int num_pair, 
    struct color_pair *pair, 
    int foreground_offset,
    int background_offset
)
{
    pair->fg_color_num += foreground_offset;
    check_color(&pair->fg_color_num);
    pair->bg_color_num += background_offset;
    check_color(&pair->bg_color_num);
    
    if (pair->fg_color_num == pair->bg_color_num) {
        pair->fg_color_num += foreground_offset;
        check_color(&pair->fg_color_num);
        pair->bg_color_num += background_offset;
        check_color(&pair->bg_color_num);
    }
    
    init_pair(num_pair, colors[pair->fg_color_num], colors[pair->bg_color_num]);
    refresh();   
}

void clear_scr(int y, int x, int height, int width)
{
    fill_scr_part(' ', y, x, height, width);
}

int main()
{
    struct square square;
    struct color_pair cl_pair;
    int rows, cols, key;
    enum { 
        square_smbl = '*',
        square_size = 5,
        key_escape = 27,
        key_space = ' ',
        delay_after_clear_screen = 300000,
        cl_pair_num = 1
    };

    initscr();
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "I can't show colors!\n");
        return 1;
    }
    
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    start_color();
    getmaxyx(stdscr,rows,cols);

    square.y = (rows - square_size) / 2;
    square.x = (cols - square_size) / 2;
    square.size = square_size;

    cl_pair.fg_color_num = colors_count - 1;
    cl_pair.bg_color_num = 0;
    init_pair(cl_pair_num, colors[cl_pair.fg_color_num], colors[cl_pair.bg_color_num]);
    attron(COLOR_PAIR(cl_pair_num));
    draw_square(&square, square_smbl);

    while ((key = getch()))
    {
        if(key == key_escape || key == key_space) 
            break;

        switch (key) {
        case KEY_UP:
            change_color(cl_pair_num, &cl_pair, 1 , 0);
            break;
        case KEY_DOWN:
            change_color(cl_pair_num, &cl_pair, -1, 0);
            break;
        case KEY_LEFT:
            change_color(cl_pair_num, &cl_pair, 0, 1);
            break;
        case KEY_RIGHT:
            change_color(cl_pair_num, &cl_pair, 0, -1);
            break;
        }
    }

    attroff(COLOR_PAIR(cl_pair_num));
    clear_scr(square.y, square.x, square.size, square.size);
    usleep(delay_after_clear_screen);
    endwin();
}