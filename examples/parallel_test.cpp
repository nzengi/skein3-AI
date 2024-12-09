#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>

void testParallelProcessing() {
    try {
        const size_t data_size = 512;
        std::vector<uint8_t> data(data_size, 0);
        
        unsigned int hw_threads = std::thread::hardware_concurrency();
        size_t num_threads = 1;

        Skein3::Config config;
        config.mode = Skein3::HashMode::STANDARD;
        config.tree_fan_out = num_threads;
        config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;
        config.size = Skein3::HashSize::HASH_256;

        auto start = std::chrono::high_resolution_clock::now();
        
        auto hash = Skein3::hash(data, config);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Processing time: " << duration.count() << " microseconds\n";
        std::cout << "Hash size: " << hash.size() << " bytes\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    testParallelProcessing();
    return 0;
} 