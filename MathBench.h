// MathBench.h
// Public interface for the math benchmarking utility.

#pragma once

// Standard library headers that are commonly useful
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <random>
#include <thread>
#include "picosha2.h"

// The MathBench class is a simple entry point for running
// different math benchmarks from your main() function.
//
// Example usage in main.cpp:
//   int main(int argc, char** argv) {
//       MathBench bench;
//       bench.run(argc, argv);
//       return 0;
//   }
class MathBench {
public:
	// Main entry instead of a free function; you can call this from main().
	// argc/argv are kept generic so you can parse CLI options later.
	int run(int argc, char** argv);

private:
    int threadCount_{1};
    //std::string selectedBenchmark_{"all"};

    // Helper to build per-thread RNGs with different seeds.
    std::random_device rd_;

    // Parse command line arguments (e.g., which benchmark to run, thread count, etc.).
    void parseArguments(int argc, char** argv);
	// Example benchmark hooks — you can change/extend these as you like.
	void runAllBenchmarks();
	void runBasicArithmeticBenchmark();
	void runTrigonometryBenchmark();
    void runLogarithmBenchmark();
    void runSha256HashingBenchmark();
    void runSortingBenchmark();
    void runMatrixMultiplicationBenchmark();

    /*
    void runExponentialBenchmark();
    void runSquareRootBenchmark();
    void runFibonacciBenchmark();
    void runPrimeNumberBenchmark();
    
    void runFourierTransformBenchmark();
    
    void runRandomNumberGenerationBenchmark();
    void runStatisticalComputationBenchmark();
    void runComplexNumberBenchmark();
    void runVectorOperationsBenchmark();
    void runDifferentialEquationBenchmark();
    void runBigIntegerBenchmark();
    ;
    void runAesEncryptionBenchmark();
    void runBaseConversionBenchmark();
    void runDateTimeComputationBenchmark();
    void runGeometryComputationBenchmark();
    void run3DTransformationBenchmark();
    void runImaginaryNumberBenchmark();
    */

    // Helper to measure how long a function takes.
    template <typename F>
    double timeFunction(F &&func, std::size_t iterations = 1'000'000)
    {
        using clock = std::chrono::high_resolution_clock;
        auto start = clock::now();
        for (std::size_t i = 0; i < iterations; ++i)
        {
            func();
        }
        auto end = clock::now();
        std::chrono::duration<double> diff = end - start;
        return diff.count(); // seconds
    }
};
