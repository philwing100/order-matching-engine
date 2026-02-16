cmake_minimum_required(VERSION 3.0)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

add_executable(untitled src/main.cpp)

add_executable(throughput_benchmark
	tests_cpp/throughput_benchmark.cpp
	src/exchange.cpp
	src/matching_engine.cpp
	src/order_book.cpp
	src/order.cpp
	src/helpers/argparse.cpp
)
