#include <iostream>
#include <cmath>

double factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

long double poisson(int k, long double lambda) {
    return exp(-lambda) * pow(lambda, k) / factorial(k);
}