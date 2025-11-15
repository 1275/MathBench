// UI.cpp
// Terminal UI implementation for benchmark display

#include "UI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

// ANSI escape codes for terminal control
#define CLEAR_SCREEN "\033[2J"
#define MOVE_CURSOR(row, col) "\033[" << (row) << ";" << (col) << "H"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define DIM "\033[2m"

UI::UI(int threadCount) 
    : threadCount_(threadCount), startTime_(std::chrono::steady_clock::now()) {
}

void UI::init() {
    clearScreen();
    hideCursor();
    startTime_ = std::chrono::steady_clock::now();
}

void UI::cleanup() {
    showCursor();
    std::cout << std::endl;
}

void UI::clearScreen() {
    std::cout << CLEAR_SCREEN << std::flush;
}

void UI::moveCursor(int row, int col) {
    std::cout << MOVE_CURSOR(row, col);
}

void UI::hideCursor() {
    std::cout << HIDE_CURSOR << std::flush;
}

void UI::showCursor() {
    std::cout << SHOW_CURSOR << std::flush;
}

void UI::startBenchmark(const std::string& name, size_t iterations) {
    currentBenchmark_ = name;
    
    // Check if benchmark already exists
    int idx = getBenchmarkIndex(name);
    if (idx == -1) {
        BenchmarkResult result;
        result.name = name;
        result.iterations = iterations;
        result.completed = false;
        benchmarks_.push_back(result);
    }
    
    refresh();
}

void UI::completeBenchmark(const std::string& name, const BenchmarkResult& result) {
    int idx = getBenchmarkIndex(name);
    if (idx != -1) {
        benchmarks_[idx] = result;
        benchmarks_[idx].completed = true;
    }
    
    currentBenchmark_ = "";
    refresh();
}

int UI::getBenchmarkIndex(const std::string& name) {
    for (size_t i = 0; i < benchmarks_.size(); ++i) {
        if (benchmarks_[i].name == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void UI::refresh() {
    clearScreen();
    drawHeader();
    drawBenchmarkList();
    drawFooter();
    std::cout << std::flush;
}

void UI::drawHeader() {
    moveCursor(1, 1);
    std::cout << BOLD << CYAN;
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗";
    moveCursor(2, 1);
    std::cout << "║" << RESET << BOLD << "                            MATHBENCH PERFORMANCE SUITE                       " << CYAN << "║";
    moveCursor(3, 1);
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << RESET;
}

void UI::drawBenchmarkList() {
    int row = 4;
    int maxRows = 18; // Leave room for header and footer
    
    // Column headers - different for single vs multi-thread
    moveCursor(row++, 1);
    if (threadCount_ == 1) {
        std::cout << BOLD << " " << padRight("Benchmark", 30) << padRight("Status", 12) 
                  << padRight("Time", 15) << padRight("Ops/sec", 18) << RESET;
    } else {
        std::cout << BOLD << " " << padRight("Benchmark", 30) << padRight("Status", 12) 
                  << padRight("Min/Max", 20) << padRight("Ops/sec", 13) << RESET;
    }
    
    moveCursor(row++, 1);
    std::cout << DIM << "────────────────────────────────────────────────────────────────────────────────" << RESET;
    
    // Benchmark rows
    for (size_t i = 0; i < benchmarks_.size() && row < maxRows + 4; ++i) {
        const auto& bench = benchmarks_[i];
        moveCursor(row++, 1);
        
        std::string shortName = truncate(bench.name, 28);
        std::cout << " " << padRight(shortName, 30);
        
        if (bench.completed) {
            std::cout << GREEN << padRight("✓ Done", 12) << RESET;
            
            if (threadCount_ == 1) {
                // Single thread: show just the time
                std::cout << padRight(formatDuration(bench.avgDuration), 15);
                std::cout << padRight(formatOpsPerSec(bench.opsPerSec), 18);
            } else {
                // Multi-thread: show min/max
                double minDuration = *std::min_element(bench.threadDurations.begin(), bench.threadDurations.end());
                double maxDuration = *std::max_element(bench.threadDurations.begin(), bench.threadDurations.end());
                std::string minMaxStr = formatDuration(minDuration) + "/" + formatDuration(maxDuration);
                std::cout << padRight(minMaxStr, 20);
                std::cout << padRight(formatOpsPerSec(bench.opsPerSec), 13);
            }
        } else if (bench.name == currentBenchmark_) {
            std::cout << YELLOW << padRight("⟳ Running...", 12) << RESET;
            if (threadCount_ == 1) {
                std::cout << padRight("---", 15);
                std::cout << padRight("---", 18);
            } else {
                std::cout << padRight("---", 20);
                std::cout << padRight("---", 13);
            }
        } else {
            std::cout << DIM << padRight("Pending", 12);
            if (threadCount_ == 1) {
                std::cout << padRight("---", 15);
                std::cout << padRight("---", 18);
            } else {
                std::cout << padRight("---", 20);
                std::cout << padRight("---", 13);
            }
            std::cout << RESET;
        }
    }
}

void UI::drawFooter() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime_).count();
    
    int completedCount = 0;
    for (const auto& bench : benchmarks_) {
        if (bench.completed) completedCount++;
    }
    
    moveCursor(22, 1);
    std::cout << CYAN << BOLD;
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << RESET;
    
    moveCursor(23, 1);
    std::cout << CYAN << "║" << RESET;
    
    std::ostringstream status;
    status << " Threads: " << threadCount_ 
           << " │ Completed: " << completedCount << "/" << benchmarks_.size()
           << " │ Elapsed: " << elapsed << "s";
    
    std::cout << padRight(status.str(), 78);
    std::cout << CYAN << "║" << RESET;
    
    moveCursor(24, 1);
    std::cout << CYAN << BOLD;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << RESET;
}

void UI::showSummary() {
    clearScreen();
    moveCursor(1, 1);
    
    std::cout << BOLD << GREEN << "═══════════════════════════════════════════════════════════════════════════════" << RESET << "\n";
    std::cout << BOLD << "                         BENCHMARK SUMMARY - ALL COMPLETE                       " << RESET << "\n";
    std::cout << BOLD << GREEN << "═══════════════════════════════════════════════════════════════════════════════" << RESET << "\n\n";
    
    double totalTime = 0.0;
    for (const auto& bench : benchmarks_) {
        if (bench.completed) {
            totalTime += bench.totalDuration;
        }
    }
    
    std::cout << " " << BOLD << "Total execution time: " << RESET << formatDuration(totalTime) << "\n";
    std::cout << " " << BOLD << "Threads used: " << RESET << threadCount_ << "\n";
    std::cout << " " << BOLD << "Benchmarks completed: " << RESET << benchmarks_.size() << "\n\n";
    
    std::cout << BOLD << " Top Performers:" << RESET << "\n";
    std::cout << DIM << " ───────────────────────────────────────────────────────────────────────────────" << RESET << "\n";
    
    // Sort by ops/sec
    std::vector<BenchmarkResult> sorted = benchmarks_;
    std::sort(sorted.begin(), sorted.end(), [](const BenchmarkResult& a, const BenchmarkResult& b) {
        return a.opsPerSec > b.opsPerSec;
    });
    
    for (size_t i = 0; i < std::min(size_t(5), sorted.size()); ++i) {
        std::cout << "  " << (i + 1) << ". " << padRight(sorted[i].name, 40) 
                  << GREEN << formatOpsPerSec(sorted[i].opsPerSec) << RESET << "\n";
    }
    
    std::cout << "\n";
    showCursor();
}

std::string UI::formatDuration(double seconds) {
    if (seconds < 0.001) {
        return std::to_string(static_cast<int>(seconds * 1000000)) + " μs";
    } else if (seconds < 1.0) {
        return std::to_string(static_cast<int>(seconds * 1000)) + " ms";
    } else {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << seconds << " s";
        return ss.str();
    }
}

std::string UI::formatOpsPerSec(double ops) {
    if (ops >= 1e9) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << (ops / 1e9) << " Gops/s";
        return ss.str();
    } else if (ops >= 1e6) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << (ops / 1e6) << " Mops/s";
        return ss.str();
    } else if (ops >= 1e3) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << (ops / 1e3) << " Kops/s";
        return ss.str();
    } else {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << ops << " ops/s";
        return ss.str();
    }
}

std::string UI::truncate(const std::string& str, size_t width) {
    if (str.length() <= width) {
        return str;
    }
    return str.substr(0, width - 3) + "...";
}

std::string UI::padRight(const std::string& str, size_t width) {
    if (str.length() >= width) {
        return str.substr(0, width);
    }
    return str + std::string(width - str.length(), ' ');
}

std::string UI::padLeft(const std::string& str, size_t width) {
    if (str.length() >= width) {
        return str.substr(0, width);
    }
    return std::string(width - str.length(), ' ') + str;
}

void UI::drawProgressBar(int row, double percentage) {
    moveCursor(row, 1);
    int barWidth = 50;
    int filled = static_cast<int>(barWidth * percentage);
    
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filled) {
            std::cout << GREEN << "█" << RESET;
        } else {
            std::cout << DIM << "░" << RESET;
        }
    }
    std::cout << "] " << static_cast<int>(percentage * 100) << "%";
}
