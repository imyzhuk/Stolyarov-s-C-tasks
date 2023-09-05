int trim(const char *str) {
    int spaces_count = 0;
    while(*str) {
        if(*str == ' ') spaces_count++;
        str++;
    };
    return spaces_count;
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