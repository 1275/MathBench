CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# Directories
SRC_DIR := src
BUILD_DIR := build
EXTERNAL_DIR := external

# Name of the final executable
TARGET := mathbench

# Source files
SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/MathBench.cpp $(SRC_DIR)/UI.cpp

# Object files (placed in build directory)
OBJS := $(BUILD_DIR)/main.o $(BUILD_DIR)/MathBench.o $(BUILD_DIR)/UI.o

# Include paths
INCLUDES := -I$(SRC_DIR) -I$(EXTERNAL_DIR)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build object files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
