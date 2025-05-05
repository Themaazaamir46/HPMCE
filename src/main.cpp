#include "matrix.hpp"
#include <iostream>
#include <chrono>

void benchmark(const Matrix& A, const Matrix& B, size_t numThreads) {
    auto start = std::chrono::high_resolution_clock::now();
    Matrix result1 = Matrix::multiplyStandard(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Standard Multiplication: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    Matrix result2 = Matrix::multiplySIMD(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD Multiplication: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    Matrix result3 = Matrix::multiplyMultiThreaded(A, B, numThreads);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-Threaded Multiplication (" << numThreads << " threads): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    if (A.getRows() == A.getCols()) {
        start = std::chrono::high_resolution_clock::now();
        Matrix result4 = Matrix::invert(A);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Matrix Inversion: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
    }
}

int main(int argc, char* argv[]) {
    try {
        size_t rowsA = 512, colsA = 512, rowsB = 512, colsB = 512;
        size_t numThreads = std::thread::hardware_concurrency();

        if (argc > 1) rowsA = colsA = rowsB = colsB = std::stoi(argv[1]);
        if (argc > 2) numThreads = std::stoi(argv[2]);

        Matrix A(rowsA, colsA), B(rowsB, colsB);
        A.randomize();
        B.randomize();

        std::cout << "Running benchmark for " << rowsA << "x" << colsB << " matrices...\n";
        benchmark(A, B, numThreads);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
