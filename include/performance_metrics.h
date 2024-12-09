#ifndef PERFORMANCE_METRICS_H
#define PERFORMANCE_METRICS_H

#include <vector>
#include <chrono>

class PerformanceMetrics {
public:
    static void recordHashOperation(unsigned long size, 
                                  std::chrono::microseconds duration);
    static void generateReport();

private:
    static std::vector<unsigned long> operation_sizes;
    static std::vector<std::chrono::microseconds> operation_durations;
};

#endif // PERFORMANCE_METRICS_H 