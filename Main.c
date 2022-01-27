//
// Created by ykb20128 on 27/01/2022.
//

#include "Main.h"
int main (void) {
    bool q = false;
    char errormsg[] = "Wrong input, either put in 0-9 or q to quit.";
    while (q == false){
        char d_buffer[30];
        printf("Gib 1 digit number ");
        scanf("%29s", d_buffer);
        int num = d_buffer[0] - 48;
        if (d_buffer[1] != '\0'){
            printf ("%s\n",errormsg);
        }
        else if (num < 10 && num >=0 ){
            print_digit(num);
        }
        else if (d_buffer[0] == 'q' || d_buffer[0] == 'Q'){
            q = true;
        }
        else {
            printf ("%s\n",errormsg);
        }
    }
    return 0;
}