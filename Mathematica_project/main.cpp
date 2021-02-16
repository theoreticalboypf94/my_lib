#include <iostream>
#include "Vector.h"

int main() {
    Vector<double> v({1,2,3,4});
    Vector<double> w({5,6,7,8});
    auto two = w * v;

    std::cout << two << "Hello, World!" << std::endl;
    return 0;
}