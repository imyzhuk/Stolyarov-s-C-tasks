int trim(const char *str) {
    if (!*str) {
        return 0;
    } else if(*str == ' ') {
        return 1 + trim(++str); 
    } else {
        return trim(++str);
    }
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