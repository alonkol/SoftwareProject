//
// Created by alon on 24/03/2016.
//

#include <stdio.h>
#include "MathIsFun.h"

int main(){
    printf("Welcome to Math Is Fun - beta version\n"
                   "Supported operations are:\n"
                   "1 - Power Calculation\n"
                   "2 - Prime Check\n"
                   "3 - Palindrome Check\n"
                   "Please enter operation number (1/2/3): \n");
    int opt, x, n, d;
    scanf("%d", &opt);


    if (opt==1){ // Power Calculation
        printf("Please enter three space separated numbers: \n");
        scanf("%d %d %d", &x, &n, &d);
        printf("res = %d\n",funPow(x,n,d));
    } else if (opt == 2){ // Prime Check
        printf("Please enter an integer: \n");
        scanf("%d", &x);
        if (funPrimeCheck(x)){
            printf("res = true\n");
        } else {
            printf("res = false\n");
        }
    } else if (opt == 3){ // Palindrome Check
        printf("Please enter an integer: \n");
        scanf("%d", &x);
        if (funPalindromeCheck(x)){
            printf("res = true\n");
        } else {
            printf("res = false\n");
        }
    } else {
        printf("Invalid input");
    }

    return 0;
}
