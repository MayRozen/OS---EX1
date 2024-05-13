#include <stdio.h>

int stackOverflow(int num){ // Infinite recursion
    return stackOverflow(num+1);
}

void divideByZero(){ // Dividing by 0
    int num = 0;
    int ans = 4/num;
}

void undefindAlloction(int size){ // Trying to access an undefined memory address
    char words[size];
    int num = 15;
    words[num] = 'h';
}

int main(int argc, char* argv[]){
    stackOverflow(*argv[1]);
    divideByZero();
    undefindAlloction(*argv[1]);
}

// Run + screenshots !