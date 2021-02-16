#include <iostream>
#include "Vector.h"
#include "Matrix.h"

int main() {
    Matrix<double> m = {{1,2}, {3,4}};
    auto b = m^-1;
    std::cout << b * m << std::endl << m * b << std::endl;
    return 0;
}