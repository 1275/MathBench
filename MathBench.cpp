#include "MathBench.h"

int MathBench::run(int argc, char **argv)
{
    parseArguments(argc, argv);
    runAllBenchmarks();
    return 0;
}

void MathBench::parseArguments(int argc, char **argv)
{
    // For now: do nothing. Defaults stay:
    //   threadCount_ = 1
    //   selectedBenchmark_ = "all"
    //
    // Later you can parse argc/argv to override them if the user provides options.
    //(void)argc;
    threadCount_ = (argc > 1) ? std::max(1, std::stoi(argv[1])) : 1;
    (void)argv;
}

void MathBench::runAllBenchmarks()
{
    runBasicArithmeticBenchmark();
    // runTrigonometryBenchmark();
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
            products[i] = product;
        });
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
    std::cout << "Total time taken across all threads: " << totalDuration << " seconds\n";
}

void MathBench::runTrigonometryBenchmark()
{
    std::cout << "Running trigonometry benchmark...\n";
}