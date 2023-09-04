int get_and_zero(int *num) {
    int prevValue = *num;
    *num = 0;
    return prevValue;
};

int main() {
    int testVar = 10;
    int testValue = get_and_zero(&testVar);
    if(testValue == 10 && testVar == 0) {
        return 0;
    } else {
        return 1;
    }
};