
#include <stdio.h>
#include <stdlib.h>

void simple_guessing_game(void);

/*
 * 
 */
int main(int argc, char** argv) {
    simple_guessing_game();
    return (EXIT_SUCCESS);
}


/* 
 * Name: void while_loops_02(void)
 * Desc: Simple game of guessing
 *      Demonstration an interactive way to use while loops
 *      
 */
void simple_guessing_game(void){
    int valid;
    int first;
    int second;
    int chosen; // chosen by computer
    int guess;
    int count_current;
    int count_max;
    
    valid = 0; // valid is false
    count_current = 0;
    count_max = 3; // consider asking user how many times they'd like to guess
    
    // srand(time(0));
    srand(time(NULL)); // Creates the seed for the random number generator.
    
    printf("Let's play a guessing game!  Pick two integers.\n");
    printf("I'll pick one in between them.\n");
    printf("I'll give you %d guesses to figure out my integer!\n", count_max);
    printf("Let's go!!!\n\n");
    
    printf("Please enter the minimum integer value\n");
    scanf("%d", &first);
    // make sure minimum and maximum value are different
    while(valid == 0){
        printf("Please enter the maximum integer value\n");
        scanf("%d", &second);
        // make sure minimum and maximum value are different
        if(second > first){
            // continue
            valid = 1;
        } else{
            valid = 0;
        }
    }
    // assume the user wants to continue
    chosen = rand_range(first, second);
    valid = 0; // the user has not yet picked the correct value
    while((valid == 0) && (count_current < count_max)){
        printf("Please guess the value that I have picked!\n");
        count_current++;
        printf("Try #%d of %d\n", count_current, count_max);
        scanf("%d", &guess);
        if(guess == chosen){ // guessed correctly, won the game
            printf("Congratulations! You guessed the correct number!\n");
            valid = 1;
            // loop terminates
        } else if(guess < chosen){ // guessed a wrong number (too low)
            printf("Nope! Number is too low!\n");
            // loop continues
        } else if(guess > chosen){ // guessed a wrong number (too high)
            printf("Nope! Number is too high!\n");
            // loop continues
        }
    }
    if(valid == 0){ // lost the game
        printf("You did not guess correctly within the number of tries :( the correct number is %d ... Wanna play again?\n", chosen);
    } else{ // won the game
        printf("Wanna play again?\n");
    }
}

/* 
 * Name: int rand_range(int min, int max )
 * Desc: Demonstration of a random integer generator
 */
int rand_range(int min, int max ){    
    return (min + rand() / (RAND_MAX / (max - min + 1) + 1)); // RAND_MAX --> find max value across all random integers in the list
}

