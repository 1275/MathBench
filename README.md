# MathBench Performance Suite

A comprehensive C++ benchmarking tool for testing mathematical operations performance.

## Features

- 12 different mathematical benchmarks
- Multi-threaded support
- Clean terminal UI designed for 80x24 display
- Real-time progress tracking
- Performance summary with top performers

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

## UI Features

The terminal UI provides:
- Real-time benchmark progress with spinning indicator
- Completion status with checkmarks
- Average time per benchmark
- Operations per second (formatted as ops/s, Kops/s, Mops/s, or Gops/s)
- Total elapsed time
- Thread count display
- Final summary with top performers

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
