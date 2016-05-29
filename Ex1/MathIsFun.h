#ifndef MATHISFUN_H_
#define MATHISFUN_H_
#include <stdbool.h>

/*
 * Calculates the remainder of pow(x,n) divided by d,
 * where pow(x,n) is the power of x n times.
 * For example:
 * {@code
 *	int res1 = funPow(3,2,5); //res1 = 3^2mod5 = 9mod5 = 4
 *  int res2 = funPow(3,0,5); //res2 = 3^0mod5 = 1mod5 = 1 }
 * @param x - the base value
 * @param n - non-negative integer which is the exponent value 
 * @param d - positive integer which is the divisor value
 * @return the value of pow(x,n) mod d. 
 *  The return value is in the range {0,1...,d-1}. If n=0, then pow(x,n) = 1.
 *  
 */
int funPow(int x, int n, int d);

/*
 * Checks if x is a prime
 * @param x - the number to be checked
 * @return - true if x is a prime, false otherwise
 */
bool funPrimeCheck(int x);

/*
 * Checks if x is a palindrome.
 * A palindromic number is a number that is the same when written forwards or backwards.
 * For example:
 * 11, 121, 1331 are all palindromic numbers.
 * 112, 124, 331 are not palindromic numbers.
 * @param x - the number to be checked
 * @return - true if x is a palindrome, false otherwise
 */
bool funPalindromeCheck(int x);

#endif /* MATHISFUN_H_ */
