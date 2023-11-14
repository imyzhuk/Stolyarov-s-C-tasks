#include <stdio.h>
#include <stdlib.h>

void copy_array(double *dest, const double *src, int len)
{
    while (len)
    {
        *dest = *src;
        dest++;
        src++;
        len--;
    } 
}

void stackdbl_init(double **stack)
{ 
    *stack = malloc(sizeof(double));
    **stack = 1;
}

void stackdbl_destroy(double **stack)
{
    free(*stack);
    *stack = NULL;
}
void stackdbl_push(double **stack, double num)
{
    double *tmp;
    int new_stack_len = **stack + 1;

    tmp = malloc(sizeof(double) * (new_stack_len));
    copy_array(tmp, *stack, new_stack_len);
    tmp[new_stack_len - 1] = num;
    tmp[0]++;

    free(*stack);
    *stack = tmp;
}
int stackdbl_pop(double **stack, double *num)
{
    double *tmp;
    int new_stack_len;
    if (**stack == 1)
    {
        return -1; //if the stack is empty
    }
    
    new_stack_len = **stack - 1;
    tmp = malloc(sizeof(double) * new_stack_len);
    copy_array(tmp, *stack, new_stack_len);
    *num = (*stack)[new_stack_len];
    tmp[0]--;

    free(*stack);
    *stack = tmp;
    return 0;
}
int stackdbl_empty(double *stack)
{
    if (stack[0] <= 1) {
        return 0; //the stack is empty
    } else {
        return -1; //the stack isn't empty
    }
}

int main()
{
    double *stack;
    double num = 10.2;
    double num2 = 15.2;
    double new_num;

    stackdbl_init(&stack);

    if(stackdbl_empty(stack) == -1) {
        fprintf(stderr, "stackdbl_empty don't work correctly\n");
        return 1;
    }

    stackdbl_push(&stack, num);
    stackdbl_push(&stack, num2);

    if(stackdbl_empty(stack) == 0) {
        fprintf(stderr, "stackdbl_empty don't work correctly\n");
        return 1;
    } else {
        printf("stackdbl_empty work correctly\n");
    }

    stackdbl_pop(&stack, &new_num);
    if (new_num != num2) {
        fprintf(stderr, "Push or pop don't work correctly!\n");
        return 2;
    }
    stackdbl_pop(&stack, &new_num);
        if (new_num != num) {
        fprintf(stderr, "Push or pop don't work correctly!\n");
        return 2;
    }

    printf("Push and pop work correctly!\n");

    stackdbl_destroy(&stack);
    if (stack) {
        fprintf(stderr, "Stack was not destroyed properly.\n");
        return 3;
    } else {
        printf("Stack was destroyed properly.\n");
    }
}