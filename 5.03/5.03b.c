#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct file_indicators{
    int min;
    int max;
    int count;
} file_indicators;

void get_file_indicators(int fd, file_indicators **fi)
{
    int num;
    file_indicators *tmp = malloc(sizeof(file_indicators));
    tmp->count = 0;

    while (read(fd, &num, sizeof(int)))
    {
        if (tmp->count == 0) {
            tmp->max = tmp->min = num;
        } else {
            if (tmp->max < num) {
                tmp->max = num;
            } else if (tmp->min > num) {
                tmp->min = num;
            }
        }

        tmp->count++;
    }

    *fi = tmp; 
}

int main(int argc, char **argv)
{
    const char args_error[] = "Too few arguments\n";
    int file_desc;
    FILE *out_file;
    file_indicators *fi;

    if (argc < 3) {
        fprintf(stderr, args_error);
        return 1;
    }

    out_file = fopen(argv[argc - 1], "w");
    if(!out_file) {
        perror(argv[argc - 1]);
        return 2;
    }

    for (int i = 1; i < argc - 1; i++)
    {
        file_desc = open(argv[i], O_RDONLY);
        if(file_desc == -1) {
            perror(argv[i]);
            return 1;
        }

        get_file_indicators(file_desc, &fi);
        fprintf(
            out_file, 
            "%s: %d (count), %d (min), %d (max)\n", 
            argv[i], 
            fi->count, 
            fi->min, 
            fi->max
        );
    }  
}