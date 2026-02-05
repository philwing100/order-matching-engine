# Compiler (change to clang++ if you want)
CXX := g++

# Flags
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

# Directories
SRC_DIR := src
BIN_DIR := bin

# Filesgc
SRC := $(SRC_DIR)/main.cpp
TARGET := $(BIN_DIR)/app

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
