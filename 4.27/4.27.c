#include <ncurses.h>
#include <stdio.h>

enum {
    unselecting_color_pair = 0,
    selecting_color_pair = 1
};

void paint_row(int row_y, char *str, int color_pair_index)
{
    move(row_y, 0);
    attrset(COLOR_PAIR(color_pair_index));
    addstr(str);
}

void draw_horizontal_line(int smbl, int y, int x, int width)
{
    move(y, x);

    for (int i = 0; i < width; i++)
        addch(smbl);
}

void clear_screen(int height, int width)
{
    for (int i = 0; i < height; i++) 
    {
        move(i, 0);
        draw_horizontal_line(' ', i, 0, width);
    }
}

void scroll_screen_down(int height, int width, char **strings, int *offset)
{
    int last_row_index = height - 1;
    
    clear_screen(height, width);

    (*offset)++;

    for (int i = 0; i < last_row_index; i++)
    {
        paint_row(i, strings[1 + *offset + i], unselecting_color_pair);
    }

    paint_row(last_row_index, strings[height + *offset], selecting_color_pair);
    refresh();
}

void scroll_screen_up(int height, int width, char **strings, int *offset)
{
    clear_screen(height, width);

    (*offset)--;

    paint_row(0, strings[1 + *offset], selecting_color_pair);

    for (int i = 1; i < height; i++)
    {
        paint_row(i, strings[1 + *offset + i], unselecting_color_pair);
    }

    refresh();
}

void switch_to_new_row(int start_y, int offset_y, char *old_str, char *new_str)
{
    int new_row_y;
    paint_row(start_y, old_str, unselecting_color_pair);

    new_row_y = start_y + offset_y;
    paint_row(new_row_y, new_str, selecting_color_pair);

    refresh();
}

int check_row_moving(int curr_idx, int offset, int max_idx, int min_idx)
{
    if (offset == -1 && curr_idx == min_idx)
        return -1;

    if (offset == 1 && curr_idx + 1 == max_idx)
        return -1;

    return 0;
}

void move_row(int start_screen_y, int *curr_str_index, int offset_y, char **all_strings, int str_count, int scr_height, int scr_width, int *scroll_count)
{
    int curr_str_index_num = *curr_str_index;
    int old_row_y;
    int can_move;

    can_move = check_row_moving(curr_str_index_num, offset_y, str_count, 0);
    if(can_move == -1) {
        return;
    }

    if(offset_y == 1 && curr_str_index_num - *scroll_count + 1 == scr_height) {
        scroll_screen_down(scr_height, scr_width, all_strings, scroll_count);
        (*curr_str_index)++;
        return;
    }
    if(offset_y == -1 && curr_str_index_num - *scroll_count == 0) {
        scroll_screen_up(scr_height, scr_width, all_strings, scroll_count);
        (*curr_str_index)--;
        return;
    }

    old_row_y = start_screen_y + curr_str_index_num - *scroll_count;
    switch_to_new_row(
        old_row_y, 
        offset_y, 
        all_strings[1 + curr_str_index_num], 
        all_strings[1 + offset_y + curr_str_index_num]
    );

    curr_str_index_num += offset_y;
    *curr_str_index = curr_str_index_num;
}

void show_start_screen(int start_y, int str_count, char **strings)
{
    for (int i = 0; i < str_count; i++)
    {
        if (!i)
            paint_row(start_y + i, strings[i + 1], selecting_color_pair);
        else 
            paint_row(start_y + i, strings[i + 1], unselecting_color_pair);
    }

    refresh();
}

void check_cmd_arg(char *str, int max_len)
{
    for (int i = 0; i < max_len; i++)
    { 
        if (!str[i])
            return;
    }

    str[max_len] = '\0';
    for (int i = 1; i < 4; i++)
    {
        str[max_len - i] = '.';
    }
}

void check_cmd_args(int argc, char **argv, int max_len)
{
    for (int i = 1; i < argc; i++)
    {
        check_cmd_arg(argv[i], max_len);
    }  
}

int main(int argc, char **argv)
{
    int rows, cols, start_menu_y; 
    int str_count_on_screen, current_str_index, key;
    int scroll_count = 0;
    enum {
        key_escape = 27,
        key_enter = 10
    };

    if (argc < 3) {
        fprintf(stderr, "Too few arguments!\n");
        return 101;
    }

    if (argc > 101) {
        fprintf(stderr, "Too many arguments!\n");
        return 102;
    }

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
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    getmaxyx(stdscr, rows, cols);

    check_cmd_args(argc, argv, cols);

    start_menu_y = 0;
    str_count_on_screen = rows;

    if (argc - 1 < rows) {
        start_menu_y = (rows - argc + 1) / 2;
        str_count_on_screen = argc - 1;
    }

    show_start_screen(start_menu_y, str_count_on_screen, argv);
    current_str_index = 0;

    while ((key = getch()))
    {
        switch (key) {
        case key_escape:
            endwin();
            return 0;
        case key_enter:
            endwin();
            current_str_index++;
            return current_str_index;
        case KEY_UP:
            move_row(
                start_menu_y,
                &current_str_index,
                -1,
                argv,
                argc - 1,
                rows,
                cols,
                &scroll_count
            );
            break;
        case KEY_DOWN:
            move_row(
                start_menu_y,
                &current_str_index,
                1,
                argv,
                argc - 1,
                rows,
                cols,
                &scroll_count
            );
            break;
        }
    }    
}