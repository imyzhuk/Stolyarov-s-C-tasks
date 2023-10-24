/*It's a solution for task 2.20*/

#include <stdio.h>

int is_letter(int smbl) {
    return smbl != ' ' && smbl != '\n';
}

int main() {
    int smbl;
    int is_prev_smbl_letter = 0;

    while ((smbl = getchar()) != EOF) {
        if(!is_letter(smbl) && is_prev_smbl_letter) {
            printf(")%c", smbl);
            is_prev_smbl_letter = 0;
        } else if(is_letter(smbl) && !is_prev_smbl_letter) {
            printf("(%c", smbl);
            is_prev_smbl_letter = 1;
        } else {
            putchar(smbl);
            is_prev_smbl_letter = is_letter(smbl);
        }
    }    
}