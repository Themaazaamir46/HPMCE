# High-Performance Matrix Computation Engine (HPMCE)

A C++ library for optimized matrix operations (multiplication and inversion) designed for AI workloads, featuring SIMD (AVX) and multi-threading optimizations.

## What It Does
HPMCE provides a high-performance matrix computation library tailored for AI model execution on CPUs. It includes:
- **Matrix Multiplication**: Standard, SIMD (AVX2), and multi-threaded implementations.
- **Matrix Inversion**: Gaussian elimination with numerical stability.
- **Performance Benchmarking**: Compare execution times across implementations.
- **Memory Efficiency**: Custom memory management and move semantics.

## Performance Benchmark Results
For a 512x512 matrix on a 4-core CPU:
- Standard Multiplication: ~1200 ms
- SIMD Multiplication: ~300 ms
- Multi-Threaded (4 threads): ~400 ms
- Inversion: ~150 ms

*Note*: Results vary based on hardware (AVX2 support required).

## My Learnings
- **Multithreading**: Dividing matrix operations into thread-safe blocks improved performance but required careful synchronization to avoid race conditions.
- **SIMD**: Using AVX2 intrinsics significantly reduced computation time by processing multiple elements in parallel, though alignment and loop unrolling were critical.
- **Memory Control**: Custom allocators and move semantics minimized memory overhead, crucial for large matrices in AI workloads.
- **Numerical Stability**: Gaussian elimination required pivot checks to prevent division by zero and ensure robust inversion.

## Prerequisites
- C++17-compatible compiler (e.g., g++ 9+)
- CMake 3.10 or higher
- Linux/Unix system with AVX2 support
- Libraries: pthread

## Build Instructions
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd HPMCE
   ```
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake and build:
   ```bash
   cmake ..
   make
   ```

## Usage
- Run the main program for benchmarks:
  ```bash
  ./main [matrix_size] [num_threads]
  ```
  Example: `./main 512 4`
- Run unit tests:
  ```bash
  ./test_matrix
  ```
- Run benchmarks:
  ```bash
  ./bench_multithread [matrix_size] [num_threads]
  ```

## Project Structure
- `include/matrix.hpp`: Matrix class definition.
- `src/matrix.cpp`: Matrix operation implementations.
- `src/main.cpp`: Main program for benchmarking.
- `tests/test_matrix.cpp`: Unit tests for multiplication and inversion.
- `benchmarks/bench_multithread.cpp`: Performance benchmarks.
- `CMakeLists.txt`: Build configuration.

## Relevance to Ziroh Labs
This project aligns with Ziroh Labs' mission to optimize AI model execution on CPUs for Kompact AI. It demonstrates:
- **C++ Proficiency**: Low-level memory management, move semantics, and SIMD (AVX2).
- **Algorithm Design**: Math-heavy matrix operations with numerical stability.
- **Performance Optimization**: Multi-threading and SIMD for CPU efficiency.
- **Code Quality**: Modular, tested code with comprehensive documentation.

## License
MIT License