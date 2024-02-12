// 4.33-task-solution, but with using libraries and without using system calls

#include <stdio.h>

enum operation_smbl {
    plus = '+',
    minus = '-',
    multiply = '*',
    divide = '/'
};

struct operation {
    enum operation_smbl smbl;
    int priority;
};

enum { new_line_smbl = '\n' };
enum { max_stack_size = 1024 };

int is_operation(int c) {
    if (c == plus || c == minus || c == multiply || c == divide) 
        return 1;
    else
        return 0;
}

int last_of(int *stack)
{
    int last_el_index;

    for (last_el_index = 0; last_el_index < max_stack_size; last_el_index++)
    {
        if (stack[last_el_index] == '\0')
            break;
    }

    last_el_index--;
    return stack[last_el_index];
}

int last_index_of(int *stack)
{
    int last_el_index;

    for (last_el_index = 0; last_el_index < max_stack_size; last_el_index++)
    {
        if (stack[last_el_index] == '\0')
            return last_el_index;
    }

    return 0;
}

void stack_push(int *stack, int num)
{
    int last_index = last_index_of(stack);

    stack[last_index] = num;
    last_index++;
    stack[last_index] = '\0'; 
}

void stack_pop(int *stack, int *num)
{
    int last_index = last_index_of(stack);

    if (!last_index)
        return;  

    last_index--;
    *num = stack[last_index];
    stack[last_index] = '\0';
}

int get_priority(int c)
{
    struct operation op[] = { 
        {'+', 1}, 
        {'-', 1}, 
        {'*', 2}, 
        {'/', 2}
    };

    for (int i = 0; i < sizeof(op) / sizeof(struct operation); i++)
    {
        if (op[i].smbl == c) {
            return op[i].priority;
        }
    }

    return -1;
}

int stack_empty(int *stack)
{
    if (stack[0] == '\0')
        return 1;

    return 0;
}

int execute(int first_operand, int second_operand, enum operation_smbl operation)
{
    switch (operation) {
    case plus:
        return first_operand + second_operand;
    case minus:
        return first_operand - second_operand;
    case multiply:
        return first_operand * second_operand;
    case divide:
        return first_operand / second_operand;
    }
}

int main(int argc, char **argv)
{
    int c, last_stack_char, is_last_input_char_digit = 0;
    int last_num, prev_last_num;
    int out_stack[max_stack_size] = { 0 };
    int operations_stack[max_stack_size] = { 0 };

    while ((c = getchar()) != EOF)
    {
        if (c == new_line_smbl || c == -1) {
            while (!stack_empty(operations_stack))
            {
                stack_pop(operations_stack, &last_stack_char);
                stack_pop(out_stack, &last_num);
                stack_pop(out_stack, &prev_last_num);
                stack_push(out_stack, execute(prev_last_num, last_num, last_stack_char));
            }

            break;
        }
        
        if (c >= '0' && c <= '9') {
            c -= '0';

            if (is_last_input_char_digit) {
                stack_pop(out_stack, &last_stack_char);
                c = last_stack_char * 10 + c;
            }
            
            stack_push(out_stack, c);
            is_last_input_char_digit = 1;
            continue;
        } else {
            is_last_input_char_digit = 0;
        }

        if (c == '(') {
            stack_push(operations_stack, c);
            continue;
        }

        if (c == ')') {
            stack_pop(operations_stack, &last_stack_char);
            while (last_stack_char != '(')
            {
                stack_pop(out_stack, &last_num);
                stack_pop(out_stack, &prev_last_num);
                stack_push(out_stack, execute(prev_last_num, last_num, last_stack_char));

                stack_pop(operations_stack, &last_stack_char);
            }

            continue;
        }

        if (is_operation(c)) {
            while (get_priority(last_of(operations_stack)) >= get_priority(c))
            {
                stack_pop(operations_stack, &last_stack_char); 
                stack_pop(out_stack, &last_num);
                stack_pop(out_stack, &prev_last_num);
                stack_push(out_stack, execute(prev_last_num, last_num, last_stack_char));
            }
            stack_push(operations_stack, c);
        }
    }
    int digits_count = 0;
    int num = out_stack[0];
    char str[12];
    for (int i = 0; num; i++)  
    {
        num /= 10;
        digits_count++;
    }

    str[digits_count] = 0;

    num = out_stack[0];
    for (int i = 1; i <= digits_count; i++)
    {
        str[digits_count - i] = num % 10 + '0';
        num /= 10;
    }
    
    printf("%s\n", str);
}