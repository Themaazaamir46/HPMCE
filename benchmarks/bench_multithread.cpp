#include "matrix.hpp"
#include <iostream>
#include <chrono>

void runBenchmark(size_t size, size_t numThreads) {
    Matrix A(size, size), B(size, size);
    A.randomize();
    B.randomize();

    auto start = std::chrono::high_resolution_clock::now();
    Matrix result1 = Matrix::multiplyStandard(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Standard (" << size << "x" << size << "): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    Matrix result2 = Matrix::multiplySIMD(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD (" << size << "x" << size << "): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    Matrix result3 = Matrix::multiplyMultiThreaded(A, B, numThreads);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi-Threaded (" << size << "x" << size << ", " << numThreads << " threads): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}

int main(int argc, char* argv[]) {
    size_t size = 512;
    size_t numThreads = std::thread::hardware_concurrency();
    if (argc > 1) size = std::stoi(argv[1]);
    if (argc > 2) numThreads = std::stoi(argv[2]);

    std::cout << "Benchmarking for matrix size " << size << "x" << size << "...\n";
    runBenchmark(size, numThreads);
    return 0;
}
