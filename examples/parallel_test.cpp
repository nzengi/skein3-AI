#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>

void testParallelProcessing() {
    try {
        // Büyük veri oluştur (100MB)
        std::vector<uint8_t> large_data(100 * 1024 * 1024);
        
        // Random veri oluşturma için modern C++ yaklaşımı
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        std::generate(large_data.begin(), large_data.end(), 
                     [&]() { return static_cast<uint8_t>(dis(gen)); });

        // Tree mode konfigürasyonu
        Skein3::Config config;
        config.mode = Skein3::HashMode::TREE;
        config.tree_fan_out = 8;  // 8 thread kullan

        auto start = std::chrono::high_resolution_clock::now();
        
        auto hash = Skein3::tree_hash(large_data, config);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Sonuçları yazdır
        std::cout << "Paralel işlem süresi: " << duration.count() << " ms\n";
        std::cout << "Hash boyutu: " << hash.size() << " bytes\n";
        
        // Hash değerini yazdır
        std::cout << "Hash değeri: ";
        for (const auto& byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte);
        }
        std::cout << std::dec << std::endl;  // Onluk tabana geri dön

    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
    }
}

int main() {
    testParallelProcessing();
    return 0;
} 