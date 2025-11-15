# MathBench Performance Suite

A shitty C++ benchmarking tool for testing mathematical operations performance.
Primiarly to test ARM and RISC-V Math perfomance 

## Features

- 12 mathematical benchmarks
- "Multi"-threaded support
- UI designed for 80x24 display
- "Real-time" progress tracking

## Project Structure

```
MathBench/
├── src/               # Source files
│   ├── main.cpp       # Entry point
│   ├── MathBench.h    # Main benchmark class header
│   ├── MathBench.cpp  # Benchmark implementations
│   ├── UI.h           # Terminal UI header
│   └── UI.cpp         # Terminal UI implementation
├── build/             # Build artifacts (object files)
├── external/          # External dependencies
│   └── picosha2.h     # SHA-256 hashing library
├── Makefile           # Build configuration
└── mathbench          # Compiled executable

```

## Dependencies for Debian based systems

### Native Build
```bash
apt update && apt install make g++ gcc build-essential 
```
If you don't wanna add sudo manually

```bash
sudo apt update && sudo apt install make g++ gcc build-essential 
```

### Cross-Compilation Toolchains
For cross-compiling to different architectures:

```bash
sudo apt install \
    gcc-arm-linux-gnueabi g++-arm-linux-gnueabi \
    gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf \
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
    gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
```

## Building

### Native Build
```bash
make
```

This will:
- Create the `build/` directory if it doesn't exist
- Compile all source files
- Link the final executable `mathbench`

### Cross-Compilation

Build for specific architecture:
```bash
make armv6      # ARMv6 (Raspberry Pi Zero, Pi 1)
make armv7      # ARMv7
make armhf      # ARM Hard Float (ARMv7)
make arm64      # ARM64/AArch64 (Raspberry Pi 3/4/5, most modern ARM)
make riscv64    # RISC-V 64-bit (Milk-V, StarFive)
```

Build all architectures at once:
```bash
make all-cross
```

This will create binaries:
- `mathbench-armv6`
- `mathbench-armv7`
- `mathbench-armhf`
- `mathbench-arm64`
- `mathbench-riscv64`

## Running

Run with default settings (1 thread):
```bash
./mathbench
```

Run with multiple threads:
```bash
./mathbench 4
```

Run cross-compiled binary on target device:
```bash
# Transfer binary to target device, then:
./mathbench-arm64 4
```

## Benchmarks Included

1. **Basic Arithmetic** - Addition, multiplication operations
2. **Trigonometry** - Sin, cos, tan calculations
3. **Logarithm** - Natural logarithm computations
4. **Exponential** - Exponential function calculations
5. **Square Root** - Square root operations
6. **SHA-256 Hashing** - Cryptographic hash operations
7. **Array Sorting** - std::sort on large arrays
8. **Matrix Multiplication** - Dense matrix operations
9. **Prime Numbers (Sieve)** - Sieve of Eratosthenes algorithm
10. **Fibonacci** - Recursive Fibonacci calculation
11. **Monte Carlo Pi** - Pi estimation using random sampling
12. **Fourier Transform (DFT)** - Discrete Fourier Transform

## Cleaning

Remove build artifacts:
```bash
make clean
```

## Requirements

- C++17 compatible compiler (g++, clang++)
- POSIX-compatible terminal for UI features
- Linux/Unix-like operating system

## License

See LICENSE file for details.
