#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build
cmake --build build --target throughput_benchmark

./build/throughput_benchmark -n 500000 -d 10 -s 1000
