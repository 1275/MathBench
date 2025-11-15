// UI.h
// Terminal UI for displaying benchmark results in a clean 80x24 format

#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>

struct BenchmarkResult {
    std::string name;
    std::vector<double> threadDurations;
    double totalDuration;
    double avgDuration;
    double opsPerSec;
    size_t iterations;
    bool completed;
    
    BenchmarkResult() : totalDuration(0.0), avgDuration(0.0), opsPerSec(0.0), 
                       iterations(0), completed(false) {}
};

class UI {
public:
    UI(int threadCount);
    
    // Initialize the UI and clear the screen
    void init();
    
    // Start a new benchmark (show it as "Running...")
    void startBenchmark(const std::string& name, size_t iterations);
    
    // Update benchmark with results
    void completeBenchmark(const std::string& name, const BenchmarkResult& result);
    
    // Refresh the entire display
    void refresh();
    
    // Show final summary
    void showSummary();
    
    // Clean up and restore terminal
    void cleanup();

private:
    int threadCount_;
    std::vector<BenchmarkResult> benchmarks_;
    std::string currentBenchmark_;
    std::chrono::time_point<std::chrono::steady_clock> startTime_;
    
    // Terminal control functions
    void clearScreen();
    void moveCursor(int row, int col);
    void hideCursor();
    void showCursor();
    
    // Drawing functions
    void drawHeader();
    void drawBenchmarkList();
    void drawFooter();
    void drawProgressBar(int row, double percentage);
    
    // Helper functions
    std::string formatDuration(double seconds);
    std::string formatOpsPerSec(double ops);
    std::string truncate(const std::string& str, size_t width);
    std::string padRight(const std::string& str, size_t width);
    std::string padLeft(const std::string& str, size_t width);
    
    int getBenchmarkIndex(const std::string& name);
};
