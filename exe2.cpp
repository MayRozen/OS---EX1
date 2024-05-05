#include <iostream>
#include <cmath>
#include "exe2.hpp"

double factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

double poisson(int k, double lambda) {
    return exp(-lambda) * pow(lambda, k) / factorial(k);
}

int main(int argc,char* argv[]) {
    if(argc != 2 ){
        std::cout<<"error"<<std::endl;
        return 0;
    }
    int k = atoi(argv[2]); // מספר ההופעות של האירוע
    double lambda = atoi(argv[1]); // פרמטר התפלגות פואסנית

    std::cout << "enter the number of shows of the event (k): ";
    std::cin >> k;

    std::cout << "enter the value of lambda (lambda): ";
    std::cin >> lambda;

    double result = poisson(k, lambda);
    std::cout << "ההתפלגות הפואסונית למספר הופעות k = " << k << " וערך למבדה lambda = " << lambda << " היא: " << result << std::endl;
    return 0;
}