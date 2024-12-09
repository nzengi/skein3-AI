#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>
#include <thread>

void testParallelProcessing() {
    try {
        std::cout << "Starting parallel processing tests...\n\n";

        // Test different data sizes
        std::vector<size_t> data_sizes = {
            1024 * 1024,      // 1 MB
            10 * 1024 * 1024, // 10 MB
            50 * 1024 * 1024  // 50 MB
        };

        // Get number of available threads
        unsigned int hw_threads = std::thread::hardware_concurrency();
        std::cout << "Available hardware threads: " << hw_threads << "\n\n";

        // Test different thread configurations
        std::vector<size_t> thread_counts = {1, 2, 4, 8};

        for (size_t data_size : data_sizes) {
            std::cout << "Testing with data size: " << (data_size / (1024.0 * 1024.0)) << " MB\n";
            
            // Create test data
            std::vector<uint8_t> data(data_size);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            std::generate(data.begin(), data.end(), [&]() { return static_cast<uint8_t>(dis(gen)); });

            // Test each thread configuration
            for (size_t num_threads : thread_counts) {
                if (num_threads > hw_threads) continue;

                Skein3::Config config;
                config.mode = Skein3::HashMode::TREE;
                config.tree_fan_out = num_threads;
                config.size = Skein3::HashSize::HASH_512;

                auto start = std::chrono::high_resolution_clock::now();
                
                auto hash = Skein3::tree_hash(data, config);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                double throughput = (data_size / 1024.0 / 1024.0) / (duration.count() / 1000000.0);

                std::cout << "Threads: " << num_threads 
                         << ", Time: " << duration.count() << " microseconds"
                         << ", Throughput: " << std::fixed << std::setprecision(2) 
                         << throughput << " MB/s\n";
            }
            std::cout << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    testParallelProcessing();
    return 0;
} 