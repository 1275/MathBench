# Native compiler
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic -Wno-psabi

# Cross-compilation toolchains
CXX_ARMV6 := arm-linux-gnueabi-g++
CXX_ARMV7 := arm-linux-gnueabihf-g++
CXX_ARMHF := arm-linux-gnueabihf-g++
CXX_ARM64 := aarch64-linux-gnu-g++
CXX_RISCV64 := riscv64-linux-gnu-g++

# Architecture-specific flags
CXXFLAGS_BASE := -std=c++17 -O2 -Wall -Wextra -pedantic -Wno-psabi
CXXFLAGS_ARMV6 := $(CXXFLAGS_BASE) -march=armv6 -mfpu=vfp -mfloat-abi=softfp
CXXFLAGS_ARMV7 := $(CXXFLAGS_BASE) -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
CXXFLAGS_ARMHF := $(CXXFLAGS_BASE) -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
CXXFLAGS_ARM64 := $(CXXFLAGS_BASE) -march=armv8-a
CXXFLAGS_RISCV64 := $(CXXFLAGS_BASE) -march=rv64gc

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

.PHONY: all clean run all-cross armv6 armv7 armhf arm64 riscv64

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build object files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cross-compilation targets
.PHONY: armv6 armv7 armhf arm64 riscv64

armv6: mathbench-armv6

build/armv6:
	mkdir -p build/armv6

build/armv6/%.o: $(SRC_DIR)/%.cpp | build/armv6
	$(CXX_ARMV6) $(CXXFLAGS_ARMV6) $(INCLUDES) -c $< -o $@

mathbench-armv6: build/armv6/main.o build/armv6/MathBench.o build/armv6/UI.o
	$(CXX_ARMV6) $(CXXFLAGS_ARMV6) -o $@ $^

armv7: mathbench-armv7

build/armv7:
	mkdir -p build/armv7

build/armv7/%.o: $(SRC_DIR)/%.cpp | build/armv7
	$(CXX_ARMV7) $(CXXFLAGS_ARMV7) $(INCLUDES) -c $< -o $@

mathbench-armv7: build/armv7/main.o build/armv7/MathBench.o build/armv7/UI.o
	$(CXX_ARMV7) $(CXXFLAGS_ARMV7) -o $@ $^

armhf: mathbench-armhf

build/armhf:
	mkdir -p build/armhf

build/armhf/%.o: $(SRC_DIR)/%.cpp | build/armhf
	$(CXX_ARMHF) $(CXXFLAGS_ARMHF) $(INCLUDES) -c $< -o $@

mathbench-armhf: build/armhf/main.o build/armhf/MathBench.o build/armhf/UI.o
	$(CXX_ARMHF) $(CXXFLAGS_ARMHF) -o $@ $^

arm64: mathbench-arm64

build/arm64:
	mkdir -p build/arm64

build/arm64/%.o: $(SRC_DIR)/%.cpp | build/arm64
	$(CXX_ARM64) $(CXXFLAGS_ARM64) $(INCLUDES) -c $< -o $@

mathbench-arm64: build/arm64/main.o build/arm64/MathBench.o build/arm64/UI.o
	$(CXX_ARM64) $(CXXFLAGS_ARM64) -o $@ $^

riscv64: mathbench-riscv64

build/riscv64:
	mkdir -p build/riscv64

build/riscv64/%.o: $(SRC_DIR)/%.cpp | build/riscv64
	$(CXX_RISCV64) $(CXXFLAGS_RISCV64) $(INCLUDES) -c $< -o $@

mathbench-riscv64: build/riscv64/main.o build/riscv64/MathBench.o build/riscv64/UI.o
	$(CXX_RISCV64) $(CXXFLAGS_RISCV64) -o $@ $^

# Build all cross-compilation targets
all-cross: armv6 armv7 armhf arm64 riscv64

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) mathbench-* build/armv6 build/armv7 build/armhf build/arm64 build/riscv64
