struct item 
{
    char *str;
    int len;
};

int main()
{
    int arr[245][12];
    int (*arr1)[12];
    arr1 = arr;

    double arr2[100][10][2];
    double (*arr3)[10][2];
    arr3 = arr2;

    char arr4[5][5];
    char (*arr5)[5];
    arr5 = arr4;

    char *arr6[5][5];
    char *(*arr7)[5];
    arr7 = arr6;

    struct item *arr8[20][3];
    struct item *(*arr9)[3];
    arr9 = arr8;

    struct item *arr10[20];
    struct item **arr11;
    arr11 = arr10;
}