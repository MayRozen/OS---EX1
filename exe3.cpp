#include <iostream>
#include "exe2.hpp"


using namespace std;


int main() {
    int k[5] = {1, 10, 2, 3, 3};
    int lambda[5] = {2, 2, 2, 3, 100};

    for(size_t i = 0; i < 5; i++) {
        long double result = poisson(k[i], lambda[i]); // Call the function through the function pointer with specific arguments
        cout << result << endl;
    }

    return 0;
}
