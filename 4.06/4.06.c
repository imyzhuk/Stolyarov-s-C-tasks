int put_sum(int *firstNum, int *secondNum, int *thirdNum) {
    int sum = *firstNum + *secondNum + *thirdNum;
    *firstNum = sum;
    *secondNum = sum;
    *thirdNum = sum;
    return sum;
};

int main() {
    int a = 1, b = 2, c = 3;
    int sum = put_sum(&a, &b, &c);
    if(sum == 6 && a == 6 && b == 6 && c == 6) {
        return 0;
    } else {
        return 1;
    }
};