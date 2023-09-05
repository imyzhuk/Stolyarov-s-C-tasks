/*It's a shortened version of task 4.08 solution*/

int trim(const char *str) {
    if (!*str) return 0;
    else return (*str == ' ') + trim(str + 1); 
};

int main() {
    char *test_str = "  Hello, World!    ";
    int test_str_spaces_count = 7;
    
    int computed_spaces_count_of_test_str = trim(test_str);
    if(computed_spaces_count_of_test_str == test_str_spaces_count) {
        return 0;
    } else {
        return 1;
    }
};