#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

char *get_file_type(mode_t mode)
{
    switch (mode & S_IFMT) {
    case S_IFBLK:  
        return "block device";            
        break;
    case S_IFCHR:  
        return "character device";        
        break;
    case S_IFDIR:  
        return "directory";               
        break;
    case S_IFIFO:  
        return "FIFO/pipe";               
        break;
    case S_IFLNK:  
        return "symlink";                 
        break;
    case S_IFREG:  
        return "regular file";            
        break;
    case S_IFSOCK: 
        return "socket";                  
        break;
    default:       
        return "unknown?";                
        break;
    }
}

void print_stat(struct stat *st)
{
    printf("File type: %s\n", get_file_type(st->st_mode));
    printf("st_dev: %ld\n", st->st_dev);
    printf("st_ino: %ld\n", st->st_ino);
    printf("st_nlink: %ld\n", st->st_nlink);
    printf("st_uid: %d\n", st->st_uid);
    printf("st_gid: %d\n", st->st_gid);
    printf("st_rdev: %ld\n", st->st_rdev);
    printf("st_size: %ld\n", st->st_size);
    printf("st_blksize: %ld\n", st->st_blksize);
    printf("st_blocks: %ld\n", st->st_blocks);
    printf("st_atime: %s", ctime(&st->st_atime));
    printf("st_mtime: %s", ctime(&st->st_mtime));
    printf("st_ctime: %s", ctime(&st->st_ctime));
}

int main(int argc, char const *argv[])
{
    const char wrong_count_of_args_error[] = "Wrong count of arguments!\n";
    struct stat stat_buf;
    const char *file_name;
    int is_smbl_link_followed;

    if (argc != 2) {
        fprintf(stderr, "%s", wrong_count_of_args_error);
        return 1;
    }

    file_name = argv[1];
    if (lstat(file_name, &stat_buf) == -1) {
        perror(file_name);
        return 1;
    }
    print_stat(&stat_buf);

    if (!S_ISLNK(stat_buf.st_mode)) {
        return 0;
    }

    printf("---------\n");

    is_smbl_link_followed = stat(file_name, &stat_buf);
    if (is_smbl_link_followed == -1) {
        printf("dangling\n");
    } else {
        print_stat(&stat_buf);
    }
    return 0;
}
