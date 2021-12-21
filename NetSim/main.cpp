#include <iostream>
#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"

void print(std::vector<ElementID> a) {
    for (unsigned int i : a)
        std::cout<<i<<" ";
    std::cout<<std::endl;
}

int main() {
    std::cout << "Welcome in spiulkolot!" << std::endl;
    return 0;
}
