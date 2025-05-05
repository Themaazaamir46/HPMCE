cmake_minimum_required(VERSION 3.10)
project(HPMCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3")

include_directories(include)
add_library(matrix src/matrix.cpp)
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE matrix pthread)

add_executable(test_matrix tests/test_matrix.cpp)
target_link_libraries(test_matrix PRIVATE matrix pthread)

add_executable(bench_multithread benchmarks/bench_multithread.cpp)
target_link_libraries(bench_multithread PRIVATE matrix pthread)
