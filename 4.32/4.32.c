// solution:
// double (*(*set_sr_func3(int num, double (*(*func)(double (*)[3], int, double))[3]))(double (*)[3], int,  double))[3]


typedef double (*d3vptr)[3];
typedef d3vptr (*search_for_vec_fptr)(d3vptr, int, double);

d3vptr search_for_vec_fptr_func(d3vptr arr, int num, double num2)
{
    return arr;
}

search_for_vec_fptr set_sr_func(int num, search_for_vec_fptr func)
{
    double matrix[3][3] = {{0.0, 1.0, 2.0}, {3.0, 4.0, 5.0}, {6.0, 7.0, 8.0}};
    d3vptr matrix2 = search_for_vec_fptr_func(matrix, 1, 1.0);
    return func;
}

search_for_vec_fptr set_sr_func2(int num, double (*(*func)(double (*)[3], int, double))[3])
{
    return func;
}

double (*(*set_sr_func3(int num, double (*(*func)(double (*)[3], int, double))[3]))(double (*)[3], int,  double))[3] //solution
{
    return func;
}

int main()
{

    search_for_vec_fptr callback = search_for_vec_fptr_func;

    set_sr_func(10, callback);
    set_sr_func2(10, callback);
    set_sr_func3(10, callback);
}