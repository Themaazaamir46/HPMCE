#include "matrix.hpp"
#include <iostream>
#include <cassert>

void testMultiplication() {
    Matrix A(2, 2), B(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;

    Matrix result = Matrix::multiplyStandard(A, B);
    assert(std::abs(result(0, 0) - 19) < 1e-5);
    assert(std::abs(result(0, 1) - 22) < 1e-5);
    assert(std::abs(result(1, 0) - 43) < 1e-5);
    assert(std::abs(result(1, 1) - 50) < 1e-5);
    std::cout << "Standard multiplication test passed.\n";
}

void testInversion() {
    Matrix A(2, 2);
    A(0, 0) = 4; A(0, 1) = 7;
    A(1, 0) = 2; A(1, 1) = 6;

    Matrix inv = Matrix::invert(A);
    Matrix identity = Matrix::multiplyStandard(A, inv);
    assert(std::abs(identity(0, 0) - 1) < 1e-5);
    assert(std::abs(identity(0, 1) - 0) < 1e-5);
    assert(std::abs(identity(1, 0) - 0) < 1e-5);
    assert(std::abs(identity(1, 1) - 1) < 1e-5);
    std::cout << "Inversion test passed.\n";
}

int main() {
    testMultiplication();
    testInversion();
    std::cout << "All tests passed.\n";
    return 0;
}
