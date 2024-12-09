#include "performance_metrics.h"
#include <iostream>
#include <numeric>

// Statik üyelerin tanımı
std::vector<unsigned long> PerformanceMetrics::operation_sizes;
std::vector<std::chrono::microseconds> PerformanceMetrics::operation_durations;

void PerformanceMetrics::recordHashOperation(
    unsigned long size, 
    std::chrono::microseconds duration
) {
    operation_sizes.push_back(size);
    operation_durations.push_back(duration);
}

void PerformanceMetrics::generateReport() {
    double total_size = std::accumulate(operation_sizes.begin(), 
                                      operation_sizes.end(), 0.0);
    double total_duration = std::accumulate(operation_durations.begin(), 
                                          operation_durations.end(), 
                                          std::chrono::microseconds(0)).count();
    
    std::cout << "Performance Report:\n";
    std::cout << "Total data processed: " << total_size / (1024*1024) << " MB\n";
    std::cout << "Total time: " << total_duration / 1000.0 << " ms\n";
    std::cout << "Average throughput: " << (total_size / total_duration) << " MB/s\n";
} 