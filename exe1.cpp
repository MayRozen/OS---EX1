#include <stdio.h>

int stackOverflow(int num){ // Infinite recursion
    return stackOverflow(num+1);
}

void divideByZero(){ // Dividing by 0
    int num = 0;
    int ans = 4/num;
}

void undefindAlloction(){ // Trying to access an undefined memory address
    char words[10];
    words[15] = 'h';
}

// Run + screenshots !
