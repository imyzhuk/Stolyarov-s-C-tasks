#include <stdio.h>

int is_letter(int smbl) 
{
    return smbl != ' ' && smbl != '\n' && smbl != '(' && smbl != ')';
}

int main() 
{
    int smbl;
    int current_word_first_letter;
    int prev_smbl = ' ';
    int words_count = 0; 
    int longest_word_size = 0;
    int shortest_word_size = 0;
    int current_word_size = 0;
    int words_count_with_even_count_of_char = 0;
    int words_count_with_odd_count_of_char = 0;
    int words_count_with_count_of_char_less_than_3 = 0;
    int words_count_with_count_of_char_less_more_than_7 = 0;
    int words_count_with_first_A_and_last_z = 0;
    int size_of_longest_sequence_of_spaces = 0;
    int size_of_current_sequence_of_spaces = 0;
    int opened_brackets_count = 0; 
    int is_there_brackets_balance = 1; 
    int count_of_combinations_with_opened_and_closed_brackets = 0; /*
    Count of meeting combinations (). 
    */

    while ((smbl = getchar()) != EOF)
    {
        if(is_letter(smbl) && !is_letter(prev_smbl)){
            words_count++;
            current_word_size++;
            prev_smbl = smbl;
            current_word_first_letter = smbl;
            if(size_of_longest_sequence_of_spaces < size_of_current_sequence_of_spaces) {
                size_of_longest_sequence_of_spaces = size_of_current_sequence_of_spaces;
            }
            size_of_current_sequence_of_spaces = 0;
        } else if(is_letter(smbl) && is_letter(prev_smbl)){
            current_word_size++;
            prev_smbl = smbl;
        } else if(!is_letter(smbl) && is_letter(prev_smbl)) {
            if(current_word_size > longest_word_size) {
                longest_word_size = current_word_size;
            } 

            if((!shortest_word_size && current_word_size) 
                || (current_word_size < shortest_word_size)
            ) {
                shortest_word_size = current_word_size;
            }

            if(current_word_size % 2 == 1) {
                words_count_with_odd_count_of_char++;
            } else {
                words_count_with_even_count_of_char++;
            }

            if (current_word_size < 3) {
                words_count_with_count_of_char_less_than_3++;
            }

            if(current_word_size > 7) {
                words_count_with_count_of_char_less_more_than_7++;
            }

            if((current_word_first_letter == 'A') && (prev_smbl == 'z')) {
                words_count_with_first_A_and_last_z++;
            }
            
            prev_smbl = smbl;
            current_word_size = 0;
        }

        if(smbl == ' ') {
            size_of_current_sequence_of_spaces++;
        }

        if(prev_smbl == '(' && smbl == ')') {
            count_of_combinations_with_opened_and_closed_brackets++;
        }
        
        if(is_there_brackets_balance) {
            if(smbl == '(') opened_brackets_count++;

            if(smbl == ')') {
                opened_brackets_count--;
            }

            if(smbl == ')' && opened_brackets_count < 0) {
                is_there_brackets_balance = 0;
            } 
        }

        prev_smbl = smbl;

        if(smbl == '\n'){
            if(opened_brackets_count) {
                is_there_brackets_balance = 0;
            }

            if(size_of_longest_sequence_of_spaces < size_of_current_sequence_of_spaces) {
                size_of_longest_sequence_of_spaces = size_of_current_sequence_of_spaces;
            }

            printf("words count: %d\n" 
                "words count with even count of chars: %d\n"
                "words count with odd count of chars: %d\n" 
                "words count with count of char less than three: %d\n"
                "words count with count of char less more than seven: %d\n"
                "words count with first A and last z: %d\n" 
                "longest word size: %d\n" 
                "shortest word size: %d\n" 
                "size of longest sequence of spaces: %d\n" 
                "is there a brackets balance? %s\n" 
                "Count of (): %d\n", 
                words_count, 
                words_count_with_even_count_of_char, 
                words_count_with_odd_count_of_char,
                words_count_with_count_of_char_less_than_3 ,
                words_count_with_count_of_char_less_more_than_7,
                words_count_with_first_A_and_last_z,
                longest_word_size, 
                shortest_word_size,
                size_of_longest_sequence_of_spaces,
                is_there_brackets_balance ? "YES" : "NO",
                count_of_combinations_with_opened_and_closed_brackets
            );
        
            words_count 
                = longest_word_size 
                = shortest_word_size 
                = current_word_size 
                = words_count_with_even_count_of_char 
                = words_count_with_odd_count_of_char 
                = words_count_with_count_of_char_less_than_3 
                = words_count_with_count_of_char_less_more_than_7
                = words_count_with_first_A_and_last_z
                = size_of_longest_sequence_of_spaces 
                = size_of_current_sequence_of_spaces
                = opened_brackets_count 
                = count_of_combinations_with_opened_and_closed_brackets
                = 0;
            is_there_brackets_balance = 1;
            prev_smbl = ' ';
        }
    } 
}