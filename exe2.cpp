#include <iostream>
#include <cmath>
#include "exe2.hpp"


using namespace std;

int main(int argc,char* argv[]) {
    if(argc != 3 ){
        cout << argc << endl;
        std::cout<<"Error"<<std::endl;
        return 0;
    }
    int k = atoi(argv[2]); // The number of appearances of the event
    long double lambda = atoi(argv[1]); // Poisson distribution parameter

    long double result = poisson(k, lambda);
    std::cout << "Poisson distribution for number of appearances: " << k << " lambda value = " << lambda << " is " << result << std::endl;
    return 0;
}