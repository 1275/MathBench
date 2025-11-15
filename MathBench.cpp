#include "MathBench.h"

#include <algorithm>

int MathBench::run(int argc, char **argv)
{
    parseArguments(argc, argv);
    runAllBenchmarks();
    return 0;
}

void MathBench::parseArguments(int argc, char **argv)
{
    // Defaults: threadCount_ = 1 when no argument is provided.
    // Later you can extend this to parse more options (e.g. which benchmarks to run).
    if (argc > 1)
    {
        try
        {
            threadCount_ = std::max(1, std::stoi(argv[1]));
        }
        catch (const std::exception &)
        {
            std::cerr << "Invalid thread count '" << argv[1]
                      << "', falling back to 1 thread.\n";
            threadCount_ = 1;
        }
    }
    else
    {
        threadCount_ = 1;
    }
}

void MathBench::executeBenchmark(const std::string &title, const std::function<double(int)> &worker, std::size_t iterations)
{
    std::cout << title << '\n';

    std::vector<double> results(threadCount_, 0.0);
    std::vector<std::thread> threads;
    threads.reserve(threadCount_);

    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([i, &worker, &results]()
                             { results[i] = worker(i); });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    for (int i = 0; i < threadCount_; ++i)
    {
        totalDuration += results[i];
        if (results[i] < 1.0)
            std::cout << "Thread " << i << " duration: " << round(results[i] * 1000.0) << " ms\n";
        else
            std::cout << "Thread " << i << " duration: " << round(results[i] * 1000.0) / 1000.0 << " seconds\n";
    }
    if (totalDuration < 1.0)
    {
        std::cout << "Combined time across all threads: " << round(totalDuration * 1000.0) << " ms\n";
    }
    else
    {
        std::cout << "Combined time across all threads: " << round(totalDuration * 1000.0) / 1000.0 << " seconds\n";
    }

    double avgDuration = totalDuration / threadCount_;
    double opsPerSec = iterations / avgDuration;
    if (opsPerSec < 1e6)
    {
        std::cout << (opsPerSec) << " ops/sec (avg per thread)\n";
    }
    else
    {
        std::cout << "~" << (round(opsPerSec / 1e6)) << " million ops/sec (avg per thread)\n";
    }
}

void MathBench::runAllBenchmarks()
{
    runBasicArithmeticBenchmark();
    std::cout << "-----------------------------------\n";
    runTrigonometryBenchmark();
    std::cout << "-----------------------------------\n";
    runLogarithmBenchmark();
    std::cout << "-----------------------------------\n";
    runExponentialBenchmark();
    std::cout << "-----------------------------------\n";
    runSquareRootBenchmark();
    std::cout << "-----------------------------------\n";
    runSha256HashingBenchmark();
    std::cout << "-----------------------------------\n";
    runSortingBenchmark();
    std::cout << "-----------------------------------\n";
    runMatrixMultiplicationBenchmark();
    std::cout << "-----------------------------------\n";
    runPrimeNumberBenchmark();
    std::cout << "-----------------------------------\n";
    runFibonacciBenchmark();
    std::cout << "-----------------------------------\n";
    runMonteCarloPiBenchmark();
    std::cout << "-----------------------------------\n";
    runFourierTransformBenchmark();
}

void MathBench::runBasicArithmeticBenchmark()
{
    const std::size_t iterations = 10'000'000;
    std::vector<double> sums(threadCount_, 0.0);
    std::vector<double> products(threadCount_, 0.0);
    executeBenchmark("Running basic arithmetic benchmark...", [this, iterations, &sums, &products](int threadIndex)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());

                         auto randDouble = [&localEngine]()
                         {
                             return (localEngine() % 1'000'000) / 100.0;
                         };

                         double sum = 0.0;
                         double product = 1.0;
                         double a = randDouble();
                         double b = randDouble();
                         
                         auto func = [&]()
                         {
                             // Make it harder to optimize away
                             a = a * 1.0001 + 0.5;
                             b = b * 0.9999 + 0.3;
                             sum += a + b;
                             product *= (a * b) / (iterations * 0.0000001); // Prevent overflow
                         };

                         double duration = timeFunction(func, iterations);
                         sums[threadIndex] = sum;
                         products[threadIndex] = product;
                         return duration; }, iterations);
}

void MathBench::runTrigonometryBenchmark()
{
    const std::size_t iterations = 1'000'000;
    std::vector<double> sines(threadCount_, 0.0);
    std::vector<double> cosines(threadCount_, 0.0);
    std::vector<double> tangents(threadCount_, 0.0);
    executeBenchmark("Running trigonometry benchmark...", [this, iterations, &sines, &cosines, &tangents](int threadIndex)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());

                         auto randAngle = [&localEngine]()
                         {
                             return (localEngine() % 36'000) / 100.0;
                         };

                         double angle = randAngle();
                         double accSine = 0.0;
                         double accCosine = 0.0;
                         double accTangent = 0.0;

                         auto func = [&]()
                         {
                             double rad = angle * M_PI / 180.0;
                             accSine += std::sin(rad);
                             accCosine += std::cos(rad);
                             accTangent += std::tan(rad);
                             angle += 0.001;
                         };

                         double duration = timeFunction(func, iterations);
                         sines[threadIndex] = accSine;
                         cosines[threadIndex] = accCosine;
                         tangents[threadIndex] = accTangent;
                         return duration; }, iterations);
}

void MathBench::runLogarithmBenchmark()
{
    const std::size_t iterations = 1'000'000;
    std::vector<double> logSums(threadCount_, 0.0);
    executeBenchmark("Running logarithm benchmark...", [this, iterations, &logSums](int threadIndex)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());

                         auto randDouble = [&localEngine]()
                         {
                             return 1.0 + (localEngine() % 1'000'000) / 100.0;
                         };

                         double sumLogs = 0.0;
                         auto func = [&]()
                         {
                             double val = randDouble();
                             sumLogs += std::log(val);
                         };

                         double duration = timeFunction(func, iterations);
                         logSums[threadIndex] = sumLogs;
                         return duration; }, iterations);
}

void MathBench::runSha256HashingBenchmark()
{
    const std::size_t iterations = 100'000;
    executeBenchmark("Running SHA-256 hashing benchmark...", [this, iterations](int)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());
                         std::uniform_int_distribution<uint8_t> byteDist(0, 255);

                         auto randData = [&localEngine, &byteDist](std::size_t size)
                         {
                             std::vector<uint8_t> data(size);
                             for (auto &b : data)
                             {
                                 b = byteDist(localEngine);
                             }
                             return data;
                         };

                         double duration = timeFunction([&]()
                                                        {
                             auto data = randData(256);
                             std::vector<unsigned char> hash(picosha2::k_digest_size);
                             picosha2::hash256(data.begin(), data.end(), hash.begin(), hash.end()); }, iterations);

                         return duration; }, iterations);
}

void MathBench::runSortingBenchmark()
{
    const std::size_t iterations = 100;  // Number of sorts per thread
    const std::size_t dataSize = 100000; // Size of each array to sort
    executeBenchmark("Running sorting benchmark...", [this, iterations, dataSize](int)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());
                         std::uniform_int_distribution<int> intDist(0, 1'000'000);

                         auto randData = [&localEngine, &intDist, dataSize]()
                         {
                             std::vector<int> data(dataSize);
                             for (auto &val : data)
                             {
                                 val = intDist(localEngine);
                             }
                             return data;
                         };

                         double duration = timeFunction([&]()
                                                        {
                             auto data = randData();
                             std::sort(data.begin(), data.end()); }, iterations);

                         return duration; }, iterations);
}

void MathBench::runMatrixMultiplicationBenchmark()
{
    const std::size_t iterations = 100;
    const std::size_t matrixSize = 100; // 200x200 matrices
    executeBenchmark("Running matrix multiplication benchmark...", [this, iterations, matrixSize](int)
                     {
                         std::random_device rd;
                         std::mt19937 localEngine(rd());
                         std::uniform_real_distribution<double> dist(0.0, 1.0);

                         auto randMatrix = [&localEngine, &dist, matrixSize]()
                         {
                             std::vector<std::vector<double>> matrix(matrixSize, std::vector<double>(matrixSize));
                             for (std::size_t r = 0; r < matrixSize; ++r)
                             {
                                 for (std::size_t c = 0; c < matrixSize; ++c)
                                 {
                                     matrix[r][c] = dist(localEngine);
                                 }
                             }
                             return matrix;
                         };

                         double duration = timeFunction([&]()
                                                        {
                             auto A = randMatrix();
                             auto B = randMatrix();
                             std::vector<std::vector<double>> C(matrixSize, std::vector<double>(matrixSize, 0.0));

                             for (std::size_t i = 0; i < matrixSize; ++i)
                             {
                                 for (std::size_t j = 0; j < matrixSize; ++j)
                                 {
                                     for (std::size_t k = 0; k < matrixSize; ++k)
                                     {
                                         C[i][j] += A[i][k] * B[k][j];
                                     }
                                 }
                             } }, iterations);

                         return duration; }, iterations);
}

// Sieve of Eratosthenes based prime number benchmark
void MathBench::runPrimeNumberBenchmark()
{
    const std::size_t iterations = 100;
    const std::size_t limit = 1'000'000; // Find primes up to 1,000,000
    executeBenchmark("Running prime number benchmark...", [this, iterations, limit](int)
                     {
                         double duration = timeFunction([&]()
                                                        {
                             std::vector<bool> isPrime(limit + 1, true);
                             isPrime[0] = isPrime[1] = false;
                             for (std::size_t p = 2; p * p <= limit; ++p)
                             {
                                 if (isPrime[p])
                                 {
                                     for (std::size_t multiple = p * p; multiple <= limit; multiple += p)
                                     {
                                         isPrime[multiple] = false;
                                     }
                                 }
                             } }, iterations);

                         return duration; }, iterations);
}

void MathBench::runExponentialBenchmark()
{
    const std::size_t iterations = 1'000'000;
    std::vector<double> expSums(threadCount_, 0.0);
    executeBenchmark("Running exponential benchmark...", [this, iterations, &expSums](int threadIndex)
                     {
                             std::random_device rd;
                             std::mt19937 localEngine(rd());

                             auto randDouble = [&localEngine]()
                             {
                                 return (localEngine() % 1000) / 10.0; // 0.0 to 99.9
                             };

                             double sumExps = 0.0;
                             auto func = [&]()
                             {
                                 double val = randDouble();
                                 sumExps += std::exp(val);
                             };

                             double duration = timeFunction(func, iterations);
                             expSums[threadIndex] = sumExps;
                             return duration; }, iterations);
}

void MathBench::runSquareRootBenchmark()
{
    const std::size_t iterations = 1'000'000;
    std::vector<double> sqrtSums(threadCount_, 0.0);
    executeBenchmark("Running square root benchmark...", [this, iterations, &sqrtSums](int threadIndex)
                     {
                             std::random_device rd;
                             std::mt19937 localEngine(rd());

                             auto randDouble = [&localEngine]()
                             {
                                 return (localEngine() % 1'000'000) / 100.0 + 1.0; // Avoid zero
                             };

                             double sumSqrts = 0.0;
                             auto func = [&]()
                             {
                                 double val = randDouble();
                                 sumSqrts += std::sqrt(val);
                             };

                             double duration = timeFunction(func, iterations);
                             sqrtSums[threadIndex] = sumSqrts;
                             return duration; }, iterations);
}

void MathBench::runFibonacciBenchmark()
{
    const std::size_t iterations = 40; // Calculate Fibonacci up to F(40)
    executeBenchmark("Running Fibonacci benchmark...", [this, iterations](int)
                     {
                             std::function<int(int)> fibonacci = [&fibonacci](int n)
                             {
                                 if (n <= 1)
                                     return n;
                                 return fibonacci(n - 1) + fibonacci(n - 2);
                             };

                             double duration = timeFunction([&]()
                                                            {
                                 for (std::size_t i = 0; i < iterations; ++i)
                                 {
                                     (void)fibonacci(20); // Prevent optimization
                                 } }, iterations);

                             return duration; }, iterations);
}

void MathBench::runMonteCarloPiBenchmark()
{
    const std::size_t iterations = 10'000'000; // Number of random points per iteration
    const std::size_t samples = 10; // Number of times to run the estimation
    executeBenchmark("Running Monte Carlo Pi benchmark...", [this, iterations, samples](int)
                     {
                             std::random_device rd;
                             std::mt19937 localEngine(rd());
                             std::uniform_real_distribution<double> dist(0.0, 1.0);

                             double duration = timeFunction([&]()
                                                            {
                                 std::size_t insideCircle = 0;
                                 for (std::size_t i = 0; i < iterations; ++i)
                                 {
                                     double x = dist(localEngine);
                                     double y = dist(localEngine);
                                     if (x * x + y * y <= 1.0)
                                     {
                                         ++insideCircle;
                                     }
                                 }
                                 (void)(4.0 * insideCircle / iterations); // Prevent optimization
                             }, samples);

                             return duration; }, samples);
}

void MathBench::runFourierTransformBenchmark()
{
    const std::size_t iterations = 10;    
    const std::size_t dataSize = 1 << 10; // 1024 points (reduced from 4096)
    executeBenchmark("Running Fourier Transform benchmark...", [this, iterations, dataSize](int)
                     {
                             std::random_device rd;
                             std::mt19937 localEngine(rd());
                             std::uniform_real_distribution<double> dist(0.0, 1.0);

                             auto randData = [&localEngine, &dist, dataSize]()
                             {
                                 std::vector<std::complex<double>> data(dataSize);
                                 for (auto &val : data)
                                 {
                                     val = std::complex<double>(dist(localEngine), dist(localEngine));
                                 }
                                 return data;
                             };

                             double duration = timeFunction([&]()
                                                            {
                                 auto data = randData();
                                 // Simple DFT implementation
                                 std::vector<std::complex<double>> result(dataSize);
                                 for (std::size_t k = 0; k < dataSize; ++k)
                                 {
                                     std::complex<double> sum(0.0, 0.0);
                                     for (std::size_t n = 0; n < dataSize; ++n)
                                     {
                                         double angle = -2.0 * M_PI * k * n / dataSize;
                                         sum += data[n] * std::complex<double>(cos(angle), sin(angle));
                                     }
                                     result[k] = sum;
                                 } }, iterations);

                             return duration; }, iterations);
}
