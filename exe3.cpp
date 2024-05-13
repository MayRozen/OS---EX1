#include <iostream>
#include <dlfcn.h>
#include "exe2.hpp"

typedef long double (*pois)(int, int); // Function pointer type adjusted

using namespace std;

void printPoisson(pois cal) {
    int k[5] = {1, 10, 2, 3, 3};
    int lambda[5] = {2, 2, 2, 3, 100};

    for(size_t i = 0; i < 5; i++) {
        long double result = cal(k[i], lambda[i]); // Call the function through the function pointer with specific arguments
        cout << result << endl;
    }
}

int main() {
    void* lib = dlopen("./libpoisson.so", RTLD_LAZY);
    if (!lib) {
        perror("Failed to load dynamic library: ");
        return -1;
    }

    pois cal = reinterpret_cast<pois>(dlsym(lib, "cal"));
    if (!cal) {
        perror("Failed to find symbol: ");
        dlclose(lib);
        return 1;
    }

    printPoisson(cal);

    dlclose(lib);
    return 0;
}
