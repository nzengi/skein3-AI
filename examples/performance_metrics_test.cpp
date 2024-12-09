#include "performance_metrics.h"

#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include "skein3.h"

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
            16,
            32,
            64      
        };

        for (size_t size : test_sizes) {
            try {
                auto data = std::make_unique<uint8_t[]>(size);
                std::fill_n(data.get(), size, 0);

                auto start = std::chrono::high_resolution_clock::now();

                Skein3::Config config;
                config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;
                config.size = Skein3::HashSize::HASH_256;

                std::vector<uint8_t> input(data.get(), data.get() + size);
                auto hash = Skein3::hash(input, config);

                auto end = std::chrono::high_resolution_clock::now();
                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                          start)
                        .count();

                std::cout << "Size: " << size << " bytes, "
                          << "Time: " << duration << " microseconds\n";

            } catch (const std::exception& e) {
                std::cerr << "Error at size " << size << ": " << e.what()
                          << "\n";
            }
        }
    }

    static void testLatency() {
        std::cout << "\n2. Latency Test\n";

        const int NUM_TESTS = 100;
        std::vector<double> latencies;

        try {
            std::vector<uint8_t> small_data(16, 0);

            Skein3::Config config;
            config.size = Skein3::HashSize::HASH_256;
            config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;

            for (int i = 0; i < NUM_TESTS; ++i) {
                auto start = std::chrono::high_resolution_clock::now();

                auto hash = Skein3::hash(small_data, config);

                auto end = std::chrono::high_resolution_clock::now();
                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                          start)
                        .count();

                latencies.push_back(duration);
            }

            double avg =
                std::accumulate(latencies.begin(), latencies.end(), 0.0) /
                latencies.size();
            std::cout << "Average latency: " << avg << " microseconds\n";

        } catch (const std::exception& e) {
            std::cerr << "Error during latency test: " << e.what() << "\n";
        }
    }

    static void testScalability() {
        std::cout << "\n3. Scalability Test\n";

        try {
            const size_t base_size = 16;
            std::vector<size_t> sizes = {base_size, base_size * 2,
                                         base_size * 4};

            for (size_t size : sizes) {
                std::vector<uint8_t> data(size, 0);

                auto start = std::chrono::high_resolution_clock::now();

                Skein3::Config config;
                config.size = Skein3::HashSize::HASH_256;
                auto hash = Skein3::hash(data, config);

                auto end = std::chrono::high_resolution_clock::now();
                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                          start)
                        .count();

                std::cout << "Size: " << size << " bytes, Time: " << duration
                          << " microseconds\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during scalability test: " << e.what() << "\n";
        }
    }

    static void testResourceUsage() {
        std::cout << "\n4. Resource Usage Test\n";

        std::vector<size_t> test_sizes = {
            1024,
            1024 * 2,
            1024 * 4
        };

        for (size_t size : test_sizes) {
            try {
                // Güvenli bellek ayırma
                auto data = std::make_unique<uint8_t[]>(size);
                std::fill_n(data.get(), size, 0);

                // Bellek kullanımını ölç
                size_t initial_memory = getCurrentMemoryUsage();

                Skein3::Config config;
                config.size = Skein3::HashSize::HASH_256;
                config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;

                std::vector<uint8_t> input(data.get(), data.get() + size);
                auto hash = Skein3::hash(input, config);

                size_t peak_memory = getCurrentMemoryUsage();
                size_t memory_usage = peak_memory - initial_memory;

                std::cout << "Size: " << size << " bytes\n"
                          << "Memory usage: " << memory_usage << " bytes\n";

            } catch (const std::exception& e) {
                std::cerr << "Error at size " << size << ": " << e.what()
                          << "\n";
            }
        }
    }

    static void generateReport() {
        std::cout << "\n5. Performance Report\n";

        try {
            const size_t test_size = 64;
            std::vector<uint8_t> data(test_size, 0);

            auto start = std::chrono::high_resolution_clock::now();

            Skein3::Config config;
            config.size = Skein3::HashSize::HASH_256;
            auto hash = Skein3::hash(data, config);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                      start)
                    .count();

            std::cout << "Test size: " << test_size << " bytes\n"
                      << "Processing time: " << duration << " microseconds\n";

        } catch (const std::exception& e) {
            std::cerr << "Error during report generation: " << e.what() << "\n";
        }
    }

   private:
    static size_t getCurrentMemoryUsage() {
#ifdef __linux__
        FILE* statm = fopen("/proc/self/statm", "r");
        if (statm) {
            long pages;
            if (fscanf(statm, "%ld", &pages) == 1) {
                fclose(statm);
                return pages * sysconf(_SC_PAGESIZE);
            }
            fclose(statm);
        }
#endif
        return 0;
    }

    template <typename Func>
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
                fscanf(
                    stat,
                    "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu",
                    &utime, &stime);
                fclose(stat);
                cpu_usage +=
                    (utime + stime) / (double)sysconf(_SC_CLK_TCK) * 100.0;
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