#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <thread>
#include <immintrin.h>
#include <stdexcept>

class Matrix {
private:
    float* data; // Custom memory allocation
    size_t rows, cols;
    bool owns_memory;

public:
    Matrix(size_t r, size_t c);
    ~Matrix();
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    void randomize();
    float& operator()(size_t i, size_t j);
    const float& operator()(size_t i, size_t j) const;
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    static Matrix multiplyStandard(const Matrix& A, const Matrix& B);
    static Matrix multiplySIMD(const Matrix& A, const Matrix& B);
    static Matrix multiplyMultiThreaded(const Matrix& A, const Matrix& B, size_t numThreads);
    static Matrix invert(const Matrix& A); // Gaussian elimination
};

#endif
