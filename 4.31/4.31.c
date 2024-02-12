typedef int *int_address_array_10[10];

typedef void (*fpvec_callback)(int, void*);
typedef void (*fpvec_item)(int, fpvec_callback, void*);

typedef double(*transform_double_fn)(double);

typedef void (*fvecpos_callback)(double*);
typedef int (*fvecpos_item)(fvecpos_callback);

// fn(int) return int *(*)[10] or
// fn(int) return address of [int*, int*, int*, int*, int*, int*,int*, int*, int*, int*];
int *(*f(int))[10];
int_address_array_10 *f(int);

// [function1, function2 ... function15];
void (*fpvec[15])(int, void (*)(int, void*), void*);
fpvec_item fpvec[15];

double (*(*repfptr)(double(*)(double)))(double);
transform_double_fn (*repfptr)(transform_double_fn);

int (*(*fvecpos)[4])(void (*)(double*));
fvecpos_item (*fvecpos)[4];

int main(){}