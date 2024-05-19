#include <iostream>
#include <cmath>
#ifndef EXE2_HPP
#define EXE2_HPP

double factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

long double poisson(int k, long double lambda) {
    return exp(-lambda) * pow(lambda, k) / factorial(k);
}

#endif /* EXE2_HPP */