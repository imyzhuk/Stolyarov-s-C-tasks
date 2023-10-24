/*It's a solution for task 2.21*/

#include <stdio.h>

int is_letter(int smbl) {
    return smbl != ' ' && smbl != '\n';
}

int main() {
    int smbl, prev_smbl = ' ', prev_prev_smbl = ' ', prev_prev_prev_smbl = ' ';

    while((smbl = getchar()) != EOF) {
        if (!is_letter(smbl) 
            && is_letter(prev_smbl) 
            && is_letter(prev_prev_smbl) 
            && !is_letter(prev_prev_prev_smbl)
        ) {
            printf("%c%c%c", prev_prev_smbl, prev_smbl, smbl);
        }

        prev_prev_prev_smbl = prev_prev_smbl;
        prev_prev_smbl = prev_smbl;
        prev_smbl = smbl;
    }
}