#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <fstream>

struct BenchmarkResult {
    size_t data_size;
    double single_thread_time;
    double multi_thread_time;
    double throughput;
    double memory_usage;
};

class PerformanceBenchmark {
public:
    static std::vector<BenchmarkResult> runFullBenchmark() {
        std::vector<BenchmarkResult> results;
        std::vector<size_t> test_sizes = {
            1024,            // 1 KB
            1024*1024,       // 1 MB
            10*1024*1024,    // 10 MB
            100*1024*1024,   // 100 MB
            1024*1024*1024   // 1 GB
        };

        for (const auto& size : test_sizes) {
            results.push_back(benchmarkSize(size));
        }

        return results;
    }

    static void saveResults(const std::vector<BenchmarkResult>& results, 
                          const std::string& filename) {
        std::ofstream file(filename);
        file << "Data Size(MB),Single Thread(ms),Multi Thread(ms),Throughput(MB/s),Memory(MB)\n";
        
        for (const auto& result : results) {
            file << (result.data_size / (1024.0 * 1024.0)) << ","
                 << result.single_thread_time << ","
                 << result.multi_thread_time << ","
                 << result.throughput << ","
                 << (result.memory_usage / (1024.0 * 1024.0)) << "\n";
        }
    }

private:
    static BenchmarkResult benchmarkSize(size_t size) {
        BenchmarkResult result;
        result.data_size = size;

        // Create test data
        std::vector<uint8_t> data(size);
        std::generate(data.begin(), data.end(), std::rand);

        // Single thread test
        {
            Skein3::Config config;
            config.mode = Skein3::HashMode::STANDARD;
            
            auto start = std::chrono::high_resolution_clock::now();
            auto hash = Skein3::hash(data, config);
            auto end = std::chrono::high_resolution_clock::now();
            
            result.single_thread_time = 
                std::chrono::duration<double, std::milli>(end - start).count();
        }

        // Multi thread test
        {
            Skein3::Config config;
            config.mode = Skein3::HashMode::TREE;
            config.tree_fan_out = std::thread::hardware_concurrency();
            
            auto start = std::chrono::high_resolution_clock::now();
            auto hash = Skein3::tree_hash(data, config);
            auto end = std::chrono::high_resolution_clock::now();
            
            result.multi_thread_time = 
                std::chrono::duration<double, std::milli>(end - start).count();
        }

        // Calculate throughput (MB/s)
        result.throughput = (size / (1024.0 * 1024.0)) / 
                          (result.multi_thread_time / 1000.0);

        // Calculate memory usage
        result.memory_usage = size * 1.5; // Estimated memory usage

        return result;
    }
};

int main() {
    try {
        std::cout << "Performance test starting...\n";
        
        auto results = PerformanceBenchmark::runFullBenchmark();
        PerformanceBenchmark::saveResults(results, "benchmark_results.csv");

        // Print results to the screen
        std::cout << "\nPerformance Test Results:\n";
        std::cout << std::fixed << std::setprecision(2);
        
        for (const auto& result : results) {
            std::cout << "\nData Size: " << (result.data_size / (1024.0 * 1024.0)) << " MB\n";
            std::cout << "Single Thread Time: " << result.single_thread_time << " ms\n";
            std::cout << "Multi Thread Time: " << result.multi_thread_time << " ms\n";
            std::cout << "Speedup: " << (result.single_thread_time / result.multi_thread_time) << "x\n";
            std::cout << "Throughput: " << result.throughput << " MB/s\n";
            std::cout << "Memory Usage: " << (result.memory_usage / (1024.0 * 1024.0)) << " MB\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 