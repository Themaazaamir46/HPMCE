#include "matrix.hpp"
#include <cstring>
#include <random>
#include <algorithm>

Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c), owns_memory(true) {
    data = new float[r * c](); // Zero-initialized
}

Matrix::~Matrix() {
    if (owns_memory) delete[] data;
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), owns_memory(true) {
    data = new float[rows * cols];
    std::memcpy(data, other.data, rows * cols * sizeof(float));
}

Matrix::Matrix(Matrix&& other) noexcept : data(other.data), rows(other.rows), cols(other.cols), owns_memory(true) {
    other.data = nullptr;
    other.owns_memory = false;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        if (owns_memory) delete[] data;
        rows = other.rows;
        cols = other.cols;
        owns_memory = true;
        data = new float[rows * cols];
        std::memcpy(data, other.data, rows * cols * sizeof(float));
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        if (owns_memory) delete[] data;
        data = other.data;
        rows = other.rows;
        cols = other.cols;
        owns_memory = true;
        other.data = nullptr;
        other.owns_memory = false;
    }
    return *this;
}

void Matrix::randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    for (size_t i = 0; i < rows * cols; ++i)
        data[i] = dis(gen);
}

float& Matrix::operator()(size_t i, size_t j) {
    return data[i * cols + j];
}

const float& Matrix::operator()(size_t i, size_t j) const {
    return data[i * cols + j];
}

Matrix Matrix::multiplyStandard(const Matrix& A, const Matrix& B) {
    if (A.cols != B.rows)
        throw std::invalid_argument("Matrix dimensions mismatch");
    Matrix result(A.rows, B.cols);
    for (size_t i = 0; i < A.rows; ++i)
        for (size_t j = 0; j < B.cols; ++j)
            for (size_t k = 0; k < A.cols; ++k)
                result(i, j) += A(i, k) * B(k, j);
    return result;
}

Matrix Matrix::multiplySIMD(const Matrix& A, const Matrix& B) {
    if (A.cols != B.rows)
        throw std::invalid_argument("Matrix dimensions mismatch");
    Matrix result(A.rows, B.cols);
    for (size_t i = 0; i < A.rows; ++i) {
        for (size_t j = 0; j < B.cols; ++j) {
            __m256 sum = _mm256_setzero_ps();
            for (size_t k = 0; k < A.cols; k += 8) {
                __m256 a = _mm256_loadu_ps(&A.data[i * A.cols + k]);
                __m256 b = _mm256_loadu_ps(&B.data[k * B.cols + j]);
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
            }
            float temp[8];
            _mm256_storeu_ps(temp, sum);
            result(i, j) = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];
        }
    }
    return result;
}

void multiplyThread(const Matrix& A, const Matrix& B, Matrix& result, size_t startRow, size_t endRow) {
    for (size_t i = startRow; i < endRow; ++i)
        for (size_t j = 0; j < B.cols; ++j)
            for (size_t k = 0; k < A.cols; ++k)
                result(i, j) += A(i, k) * B(k, j);
}

Matrix Matrix::multiplyMultiThreaded(const Matrix& A, const Matrix& B, size_t numThreads) {
    if (A.cols != B.rows)
        throw std::invalid_argument("Matrix dimensions mismatch");
    Matrix result(A.rows, B.cols);
    std::vector<std::thread> threads;
    size_t rowsPerThread = A.rows / numThreads;
    for (size_t t = 0; t < numThreads; ++t) {
        size_t startRow = t * rowsPerThread;
        size_t endRow = (t == numThreads - 1) ? A.rows : startRow + rowsPerThread;
        threads.emplace_back(multiplyThread, std::ref(A), std::ref(B), std::ref(result), startRow, endRow);
    }
    for (auto& thread : threads)
        thread.join();
    return result;
}

Matrix Matrix::invert(const Matrix& A) {
    if (A.rows != A.cols)
        throw std::invalid_argument("Matrix must be square");
    size_t n = A.rows;
    Matrix result(n, n);
    Matrix augmented(n, 2 * n);

    // Initialize augmented matrix [A | I]
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            augmented(i, j) = A(i, j);
        augmented(i, i + n) = 1.0f;
    }

    // Gaussian elimination
    for (size_t i = 0; i < n; ++i) {
        float pivot = augmented(i, i);
        if (std::abs(pivot) < 1e-10)
            throw std::runtime_error("Matrix is not invertible");
        for (size_t j = 0; j < 2 * n; ++j)
            augmented(i, j) /= pivot;
        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                float factor = augmented(k, i);
                for (size_t j = 0; j < 2 * n; ++j)
                    augmented(k, j) -= factor * augmented(i, j);
            }
        }
    }

    // Extract inverse
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            result(i, j) = augmented(i, j + n);
    return result;
}
