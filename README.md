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

## Building

```bash
make
```

## Dependencies for Debian based systems


```bash
apt update && apt install make g++ gcc build-essential 
```
If you don't wanna add sudo manually

```bash
sudo apt update && sudo apt get install make g++ gcc build-essential 
```


This will:
- Create the `build/` directory if it doesn't exist
- Compile all source files
- Link the final executable `mathbench`

## Running

Run with default settings (1 thread):
```bash
./mathbench
```

Run with multiple threads:
```bash
./mathbench 4
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
