#include <stddef.h>

void f()
{
    return;
}

int f1(int t)
{
    return t;
}

void *f2(int x)
{
    return NULL;
}

double f3(int x, const char *str)
{
    return 1.0;
}

void f4(double (*vecp)[3])
{
    return;
}

double (*f5(int len, double (*vecp)[3]))[3]
{
    return vecp;
}

int main()
{
    void (*p)();
    p = f;

    int (*p1)(int);
    p1 = f1;

    void *(*p2)(int);
    p2 = f2;

    double (*p3)(int, const char *);
    p3 = f3;

    void (*p4)(double (*)[3]);
    p4 = f4;

    double (*(*p5)(int, double (*)[3]))[3];
    p5 = f5;
}