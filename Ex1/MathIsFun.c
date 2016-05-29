#include "MathIsFun.h"

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int funPow(int x, int n, int d) {
    if (n == 0) return 1;

    int y;

    if (n % 2 == 0){
         y = funPow(x, n/2 , d);
        return y*y%d;
    } else {
        y = funPow(x, (n-1)/2 , d);
        if (x < 0){
            return x*y*y*(1-d)%d;
        }
        return x*y*y%d;
    }

}

int funSqrt(int x) {
    if (x < 0) return -1;

    int y = 1, i;

    while (y*y < x){
        y *= 2;
    }

	for (i = y/2; i <= x; i++){
        if (i*i <= x && (i+1)*(i+1) > x) return i;
    }

    return 0;
}

bool funPrimeCheck(int x) {
    if (x < 2) return false;

    int y = funSqrt(x), i;

    for (i = 2; i <= y; i++){
        if (x%i == 0) return false;
    }
    return true;
}

bool funPalindromeCheck(int x) {
    int num = x, rev = 0;
    while (num > 0) {
        rev = rev * 10 + num % 10;
        num /= 10;
    }
    return rev == x;
}
