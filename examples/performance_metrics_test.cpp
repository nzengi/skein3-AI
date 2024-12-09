#include "skein3.h"
#include "performance_metrics.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <numeric>

class PerformanceMetricsTest {
public:
    struct BenchmarkResult {
        size_t data_size;
        double single_thread_time;
        double multi_thread_time;
        double throughput;
        double memory_usage;
        double cpu_usage;
        size_t cache_misses;
    };

    static void runAllTests() {
        std::cout << "Performance metrics tests starting...\n\n";

        testThroughput();
        testLatency();
        testScalability();
        testResourceUsage();
        generateReport();
    }

private:
    static void testThroughput() {
        std::cout << "1. Throughput Test\n";

        std::vector<size_t> test_sizes = {
            1024 * 1024,      // 1 MB
            2 * 1024 * 1024,  // 2 MB
            5 * 1024 * 1024   // 5 MB - Daha küçük maksimum boyut
        };

        for (size_t size : test_sizes) {
            try {
                std::vector<uint8_t> data(size);
                std::generate(data.begin(), data.end(), std::rand);

                auto start = std::chrono::high_resolution_clock::now();
                
                Skein3::Config config;
                config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;
                config.size = Skein3::HashSize::HASH_512;
                
                auto hash = Skein3::hash(data, config);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start
                ).count();

                double throughput = (size / 1024.0 / 1024.0) / (duration / 1000.0);
                std::cout << "Size: " << (size / 1024.0 / 1024.0) << " MB, "
                         << "Throughput: " << throughput << " MB/s\n";
                         
            } catch (const std::exception& e) {
                std::cerr << "Error at size " << size << ": " << e.what() << "\n";
            }
        }
    }

    static void testLatency() {
        std::cout << "\n2. Latency Test\n";

        const int NUM_TESTS = 1000;
        std::vector<double> latencies;

        std::vector<uint8_t> small_data(64); // 64 byte test data
        std::generate(small_data.begin(), small_data.end(), std::rand);

        Skein3::Config config;
        
        for (int i = 0; i < NUM_TESTS; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            
            auto hash = Skein3::hash(small_data, config);
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                end - start
            ).count();

            latencies.push_back(duration);
        }

        // Calculate statistics
        double avg_latency = std::accumulate(latencies.begin(), 
                                           latencies.end(), 0.0) / NUM_TESTS;
        
        std::sort(latencies.begin(), latencies.end());
        double p95_latency = latencies[static_cast<size_t>(NUM_TESTS * 0.95)];
        double p99_latency = latencies[static_cast<size_t>(NUM_TESTS * 0.99)];

        std::cout << "Average latency: " << avg_latency << " microseconds\n";
        std::cout << "P95 latency: " << p95_latency << " microseconds\n";
        std::cout << "P99 latency: " << p99_latency << " microseconds\n";
    }

    static void testScalability() {
        std::cout << "\n3. Scalability Test\n";

        std::vector<int> thread_counts = {1, 2, 4, 8, 16};
        std::vector<uint8_t> data(100 * 1024 * 1024); // 100MB test data
        std::generate(data.begin(), data.end(), std::rand);

        double baseline_time = 0;

        for (int thread_count : thread_counts) {
            Skein3::Config config;
            config.mode = Skein3::HashMode::TREE;
            config.tree_fan_out = thread_count;

            auto start = std::chrono::high_resolution_clock::now();
            
            auto hash = Skein3::tree_hash(data, config);
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start
            ).count();

            if (thread_count == 1) {
                baseline_time = duration;
            }

            double speedup = baseline_time / duration;
            double efficiency = speedup / thread_count;

            std::cout << "Thread count: " << thread_count
                     << ", Speedup: " << speedup
                     << "x, Efficiency: " << (efficiency * 100) << "%\n";
        }
    }

    static void testResourceUsage() {
        std::cout << "\n4. Resource Usage Test\n";

        std::vector<size_t> test_sizes = {
            1024 * 1024,      // 1 MB
            10 * 1024 * 1024, // 10 MB
            100 * 1024 * 1024 // 100 MB
        };

        for (size_t size : test_sizes) {
            std::vector<uint8_t> data(size);
            std::generate(data.begin(), data.end(), std::rand);

            // Measure memory usage
            size_t initial_memory = getCurrentMemoryUsage();
            
            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
            
            size_t peak_memory = getCurrentMemoryUsage();
            size_t memory_usage = peak_memory - initial_memory;

            // Measure CPU usage
            double cpu_usage = measureCPUUsage([&]() {
                Skein3::hash(data, config);
            });

            std::cout << "Size: " << (size / 1024.0 / 1024.0) << " MB\n"
                     << "Memory usage: " << (memory_usage / 1024.0) << " KB\n"
                     << "CPU usage: " << cpu_usage << "%\n\n";
        }
    }

    static void generateReport() {
        std::cout << "\n5. Performance Report Generating...\n";

        std::ofstream report("performance_report.html");
        report << "<html><head><title>Skein3 Performance Report</title></head><body>\n";
        report << "<h1>Skein3 Performance Report</h1>\n";

        // Throughput graph
        report << "<h2>Throughput Analysis</h2>\n";
        report << "<div id='throughput_chart'></div>\n";

        // Latency graph
        report << "<h2>Latency Analysis</h2>\n";
        report << "<div id='latency_chart'></div>\n";

        // Scalability graph
        report << "<h2>Scalability Analysis</h2>\n";
        report << "<div id='scalability_chart'></div>\n";

        // Resource Usage graph
        report << "<h2>Resource Usage Analysis</h2>\n";
        report << "<div id='resource_chart'></div>\n";

        report << "</body></html>\n";
        report.close();

        std::cout << "Report generated: performance_report.html\n";
    }

private:
    static size_t getCurrentMemoryUsage() {
        #ifdef _WIN32
            PROCESS_MEMORY_COUNTERS_EX pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), 
                               (PROCESS_MEMORY_COUNTERS*)&pmc, 
                               sizeof(pmc));
            return pmc.WorkingSetSize;
        #elif defined(__linux__)
            FILE* statm = fopen("/proc/self/statm", "r");
            if (statm) {
                long pages;
                if (fscanf(statm, "%ld", &pages) == 1) {
                    fclose(statm);
                    return pages * getpagesize();
                }
                fclose(statm);
            }
            return 0;
        #else
            return 0;
        #endif
    }

    template<typename Func>
    static double measureCPUUsage(Func&& func) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Start the process
        std::thread worker(std::forward<Func>(func));
        
        // Measure CPU usage
        double cpu_usage = 0.0;
        int samples = 0;
        
        while (worker.joinable()) {
            // Measure CPU usage
            #ifdef __linux__
                FILE* stat = fopen("/proc/self/stat", "r");
                if (stat) {
                    unsigned long utime, stime;
                    fscanf(stat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu",
                           &utime, &stime);
                    fclose(stat);
                    cpu_usage += (utime + stime) / (double)sysconf(_SC_CLK_TCK) * 100.0;
                    samples++;
                }
            #endif
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        worker.join();
        
        return samples > 0 ? cpu_usage / samples : 0.0;
    }
};

int main() {
    try {
        PerformanceMetricsTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 