#include "skein3.h"
#include "performance_metrics.h"
#include <iostream>
#include <iomanip>
#include <chrono>

void testPerformance() {
    // Test data sizes
    std::vector<size_t> test_sizes = {1024, 1024*1024, 10*1024*1024}; // 1KB, 1MB, 10MB

    for (size_t size : test_sizes) {
        // Create test data
        std::vector<uint8_t> data(size, 'x');

        // Measure time
        auto start = std::chrono::high_resolution_clock::now();
        
        Skein3::Config config;
        auto hash = Skein3::hash(data, config);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // Record metrics
        PerformanceMetrics::recordHashOperation(size, duration);

        std::cout << "Data size: " << size << " bytes\n";
        std::cout << "Operation time: " << duration.count() << " microseconds\n";
        std::cout << std::fixed << std::setprecision(2)
                 << "Throughput: " << (size * 1000000.0 / duration.count()) 
                 << " bytes/second\n\n";
    }

    // Generate performance report
    PerformanceMetrics::generateReport();
}

int main() {
    testPerformance();
    return 0;
} 