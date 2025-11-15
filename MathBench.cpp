#include "MathBench.h"

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

void MathBench::runAllBenchmarks()
{
    runBasicArithmeticBenchmark();
    std::cout << "-----------------------------------\n";
    runTrigonometryBenchmark();
    std::cout << "-----------------------------------\n";
    runLogarithmBenchmark();
    std::cout << "-----------------------------------\n";
    runSha256HashingBenchmark();
    std::cout << "-----------------------------------\n";
    runSortingBenchmark();
}

void MathBench::runBasicArithmeticBenchmark()
{
    std::cout << "Running basic arithmetic benchmark...\n";
    const std::size_t iterations = 1'000'000;
    std::vector<double> results(threadCount_, 0.0);
    std::vector<double> sums(threadCount_, 0.0);
    std::vector<double> products(threadCount_, 0.0);

    std::vector<std::thread> threads;
    threads.reserve(threadCount_);

    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([this, iterations, &results, &sums, &products, i]()
                             {
            std::random_device rd;
            std::mt19937 localEngine(rd());

            auto randDouble = [&localEngine]() {
                return (localEngine() % 1000000) / 100.0;
            };

            double a = randDouble();
            double b = randDouble();
            double sum = 0.0;
            double product = 0.0;
            auto func = [&]() {
                sum += a + b;
                product += a * b;
            };

            double duration = timeFunction(func, iterations);
            results[i] = duration;
            sums[i] = sum;
            products[i] = product; });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    int threadIndex = 0;
    for (const auto &dur : results)
    {
        totalDuration += dur;
        std::cout << "Thread " << threadIndex++ << " duration: " << dur << " seconds\n";
    }
    std::cout << "Combined time across all threads: " << totalDuration << " seconds\n";
}

void MathBench::runTrigonometryBenchmark()
{
    std::cout << "Running trigonometry benchmark...\n";
    const std::size_t iterations = 1'000'000;
    std::vector<double> results(threadCount_, 0.0);
    std::vector<double> sines(threadCount_, 0.0);
    std::vector<double> cosines(threadCount_, 0.0);
    std::vector<double> tangents(threadCount_, 0.0);
    std::vector<std::thread> threads;
    threads.reserve(threadCount_);
    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([this, iterations, &results, &sines, &cosines, &tangents, i]()
                             {
            std::random_device rd;
            std::mt19937 localEngine(rd());

            auto randAngle = [&localEngine]() {
                return (localEngine() % 36000) / 100.0; // 0.0 to 360.0 degrees
            };

            double angle = randAngle();   // in degrees
            double accSine = 0.0;
            double accCosine = 0.0;
            double accTangent = 0.0;

            // Use radians for std::sin/cos/tan
            auto func = [&]() {
                double rad = angle * M_PI / 180.0;
                accSine   += std::sin(rad);
                accCosine += std::cos(rad);
                accTangent += std::tan(rad);
                angle += 0.001; // change the angle slightly each iteration
            };

            double duration = timeFunction(func, iterations);
            results[i] = duration;
            sines[i] = accSine;
            cosines[i] = accCosine;
            tangents[i] = accTangent; });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    int threadIndex = 0;
    for (const auto &dur : results)
    {
        totalDuration += dur;
        std::cout << "Thread " << threadIndex++ << " duration: " << dur << " seconds\n";
    }
    std::cout << "Combined time across all threads: " << totalDuration << " seconds\n";
}

void MathBench::runLogarithmBenchmark()
{
    std::cout << "Running logarithm benchmark...\n";
    const std::size_t iterations = 1'000'000;
    std::vector<double> results(threadCount_, 0.0);
    std::vector<double> logSums(threadCount_, 0.0);

    std::vector<std::thread> threads;
    threads.reserve(threadCount_);

    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([this, iterations, &results, &logSums, i]()
                             {
            std::random_device rd;
            std::mt19937 localEngine(rd());

            auto randDouble = [&localEngine]() {
                return 1.0 + (localEngine() % 1000000) / 100.0; // avoid log(0)
            };

            double sumLogs = 0.0;
            auto func = [&]() {
                double val = randDouble();
                sumLogs += std::log(val);
            };

            double duration = timeFunction(func, iterations);
            results[i] = duration;
            logSums[i] = sumLogs; });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    int threadIndex = 0;
    for (const auto &dur : results)
    {
        totalDuration += dur;
        std::cout << "Thread " << threadIndex++ << " duration: " << dur << " seconds\n";
    }
    std::cout << "Combined time across all threads: " << totalDuration << " seconds\n";
}

void MathBench::runSha256HashingBenchmark()
{
    std::cout << "Running SHA-256 hashing benchmark...\n";
    const std::size_t iterations = 100'000;
    std::vector<double> results(threadCount_, 0.0);

    std::vector<std::thread> threads;
    threads.reserve(threadCount_);

    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([this, iterations, &results, i]()
                            {
            std::random_device rd;
            std::mt19937 localEngine(rd());
            std::uniform_int_distribution<uint8_t> byteDist(0, 255);

            auto randData = [&localEngine, &byteDist](std::size_t size) {
                std::vector<uint8_t> data(size);
                for (auto &b : data)
                {
                    b = byteDist(localEngine);
                }
                return data;
            };

            double duration = timeFunction([&]() {
                auto data = randData(256); // 256 bytes of random data

                std::vector<unsigned char> hash(picosha2::k_digest_size);
                picosha2::hash256(data.begin(), data.end(), hash.begin(), hash.end());

                // Optional: accumulate something from hash so the compiler can't drop it
                // (though the work is already pretty observable).
            }, iterations);
            results[i] = duration; });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    int threadIndex = 0;
    for (const auto &dur : results)
    {
        totalDuration += dur;
        std::cout << "Thread " << threadIndex++ << " duration: " << dur << " seconds\n";
    }
    std::cout << "Combined time across all threads: " << totalDuration << " seconds\n";
}

void MathBench::runSortingBenchmark()
{
    std::cout << "Running sorting benchmark...\n";
    const std::size_t iterations = 1000; // Number of sorts per thread
    const std::size_t dataSize = 100000;   // Size of each array to sort
    std::vector<double> results(threadCount_, 0.0);

    std::vector<std::thread> threads;
    threads.reserve(threadCount_);

    for (int i = 0; i < threadCount_; ++i)
    {
        threads.emplace_back([this, iterations, dataSize, &results, i]()
                             {
            std::random_device rd;
            std::mt19937 localEngine(rd());
            std::uniform_int_distribution<int> intDist(0, 1000000);

            auto randData = [&localEngine, &intDist, dataSize]() {
                std::vector<int> data(dataSize);
                for (auto &val : data)
                {
                    val = intDist(localEngine);
                }
                return data;
            };

            double duration = timeFunction([&]() {
                auto data = randData();
                std::sort(data.begin(), data.end());
            }, iterations);
            results[i] = duration; });
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    double totalDuration = 0.0;
    int threadIndex = 0;
    for (const auto &dur : results)
    {
        totalDuration += dur;
        std::cout << "Thread " << threadIndex++ << " duration: " << dur << " seconds\n";
    }
    std::cout << "Combined time across all threads: " << totalDuration << " seconds\n";
}