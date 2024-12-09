#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>

void testParallelProcessing() {
    try {
        // Daha küçük veri boyutu kullan
        const size_t data_size = 10 * 1024 * 1024; // 10MB
        std::vector<uint8_t> large_data(data_size);
        
        // Güvenli veri oluşturma
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        for (size_t i = 0; i < data_size; ++i) {
            large_data[i] = static_cast<uint8_t>(dis(gen));
        }

        // Thread sayısını donanıma göre ayarla
        unsigned int hw_threads = std::thread::hardware_concurrency();
        size_t num_threads = hw_threads > 0 ? std::min(size_t(8), size_t(hw_threads)) : 4;

        Skein3::Config config;
        config.mode = Skein3::HashMode::TREE;
        config.tree_fan_out = num_threads;
        config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;

        auto start = std::chrono::high_resolution_clock::now();
        
        auto hash = Skein3::tree_hash(large_data, config);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        );

        std::cout << "Parallel processing time: " << duration.count() << " ms\n";
        std::cout << "Hash size: " << hash.size() << " bytes\n";
        std::cout << "Using " << num_threads << " threads\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    testParallelProcessing();
    return 0;
} 