#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>

void testParallelProcessing() {
    try {
        // Create large data (100MB)
        std::vector<uint8_t> large_data(100 * 1024 * 1024);
        
        // Modern C++ approach for random data generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        std::generate(large_data.begin(), large_data.end(), 
                     [&]() { return static_cast<uint8_t>(dis(gen)); });

        // Tree mode configuration
        Skein3::Config config;
        config.mode = Skein3::HashMode::TREE;
        config.tree_fan_out = 8;  // 8 threads used

        auto start = std::chrono::high_resolution_clock::now();
        
        auto hash = Skein3::tree_hash(large_data, config);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Print results
        std::cout << "Parallel processing time: " << duration.count() << " ms\n";
        std::cout << "Hash size: " << hash.size() << " bytes\n";
        
        // Print hash value
        std::cout << "Hash value: ";
        for (const auto& byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte);
        }
        std::cout << std::dec << std::endl;  // Return to decimal

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    testParallelProcessing();
    return 0;
} 