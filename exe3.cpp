#include <iostream>
#include <dlfcn.h>
#include <vector>
<<<<<<< HEAD
#include "exe2.hpp"
=======
>>>>>>> b8d99493a0b5f092a7a7ba2baba222cfc26ada2e

typedef long double (*pois)();
long double poisson(int k, long double lambda);
void printPoisson();

int main() {
    void* lib = dlopen("./libpoisson.so", RTLD_LAZY);
    if (!lib) {
        std::cerr << "Failed to load dynamic library: " << dlerror() << std::endl;
        return -1;
    }

    pois cal = reinterpret_cast<pois>(dlsym(lib, "cal"));
    if (!cal) {
        std::cerr << "Failed to find symbol: " << dlerror() << std::endl;
        dlclose(lib);
        return 1;
    }

    cal();

    printPoisson();

    dlclose(lib);
    return 0;
}

void printPoisson(){ // Print all the results of poisson's calculations
    std::vector<int> k = {1, 10, 2, 3, 3};
    std::vector<long double> lambda = {2, 2, 2, 3, 100};

<<<<<<< HEAD
    for(decltype(k.size()) i = 0; i < k.size(); i++) {
        long double result = poisson(k[i], lambda[i]);
        std::cout << result << std::endl;
    }
}
=======
    for(int i=0; i<5; i++){
        long double result = poisson(k[i], lambda[i]);
        std::cout<<result<<std::endl;
    }
}
>>>>>>> b8d99493a0b5f092a7a7ba2baba222cfc26ada2e
