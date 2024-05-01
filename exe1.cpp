#include <stdio.h>

int stackOverflow(int num){//infinite recursion
    return stackOverflow(num+1);
}

void divideByZero(){//dividing by 0
    int num = 0;
    int ans = 4/num;
}

void undefindAlloction(){
    char words[10];
    words[15] = 'h';
}
