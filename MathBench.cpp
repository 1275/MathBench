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
    std::cout << "-----------------------------------\n";
    runTrigonometryBenchmark();
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
            tangents[i] = accTangent;
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