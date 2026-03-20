cmake_minimum_required(VERSION 3.5)
project(order_matching_engine)

set(CMAKE_CXX_STANDARD 23)

# Optimise for the current machine's ISA (enables AVX2/AVX-512 where
# available).  -O3 over -O2 for more aggressive inlining and loop
# transforms.  -fno-omit-frame-pointer keeps stack frames visible in
# profilers without meaningful overhead.
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-omit-frame-pointer")

# Default to Release if the caller didn't specify.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(untitled src/main.cpp)

add_executable(throughput_benchmark
    tests_cpp/throughput_benchmark.cpp
    src/exchange.cpp
    src/matching_engine.cpp
    src/order_book.cpp
    src/order.cpp
    src/helpers/argparse.cpp
)

add_executable(order_book_tests
    tests_cpp/order_book_tests.cpp
    src/order_book.cpp
    src/order.cpp
)
