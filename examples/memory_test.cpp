#include "skein3.h"
#include "memory_protection.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <memory>
#include <cstring>
#include <thread>
#include <random>
#include <cassert>
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#endif

class MemoryTest {
public:
    struct MemoryMetrics {
        size_t allocated_memory;
        size_t peak_memory;
        size_t memory_leaks;
        double secure_cleanup_time;
        bool protection_active;
    };

    static void runMemoryTests() {
        std::cout << "Memory tests starting...\n\n";

        testBasicMemoryOperations();
        testSecureMemoryProtection();
        testMemoryLeaks();
        testConcurrentMemoryAccess();
        testMemoryBoundaries();
    }

private:
    static void testBasicMemoryOperations() {
        std::cout << "1. Basic Memory Operations Test\n";
        
        try {
            // Different memory allocations
            std::vector<size_t> test_sizes = {
                1024,            // 1 KB
                1024*1024,       // 1 MB
                10*1024*1024,    // 10 MB
                100*1024*1024    // 100 MB
            };

            for (size_t size : test_sizes) {
                auto metrics = testAllocation(size);
                
                std::cout << "\nSize: " << (size / 1024.0 / 1024.0) << " MB\n";
                std::cout << "Allocated: " << metrics.allocated_memory << " bytes\n";
                std::cout << "Peak usage: " << metrics.peak_memory << " bytes\n";
                std::cout << "Cleanup time: " << metrics.secure_cleanup_time << " ms\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    static void testSecureMemoryProtection() {
        std::cout << "\n2. Secure Memory Protection Test\n";
        
        try {
            const size_t test_size = 1024;
            auto secure_memory = std::make_unique<uint8_t[]>(test_size);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<uint8_t> dis;
            
            for (size_t i = 0; i < test_size; i++) {
                secure_memory[i] = dis(gen);
            }

            QuantumResistantMemory::ProtectionConfig config{
                .level = QuantumResistantMemory::ProtectionLevel::ENHANCED,
                .rounds = 3,
                .secure_wipe = true
            };

            std::vector<uint8_t> canary(32, 0xAA);
            std::memcpy(secure_memory.get(), canary.data(), canary.size());
            std::memcpy(secure_memory.get() + test_size - canary.size(), 
                       canary.data(), canary.size());

            bool integrity_check = QuantumResistantMemory::verifyIntegrity(
                secure_memory.get(),
                test_size,
                config
            );

            if (!integrity_check) {
                std::cerr << "Memory integrity check failed\n";
            } else {
                std::cout << "Memory integrity check passed\n";
            }

            QuantumResistantMemory::secureWipe(secure_memory.get(), test_size);

        } catch (const std::exception& e) {
            std::cerr << "Secure memory error: " << e.what() << "\n";
        }
    }

    static void testMemoryLeaks() {
        std::cout << "\n3. Memory Leak Test\n";
        
        const int NUM_ITERATIONS = 1000;
        size_t initial_memory = getCurrentMemoryUsage();
        
        for (int i = 0; i < NUM_ITERATIONS; ++i) {
            // Memory allocation and hash operation
            std::vector<uint8_t> data(1024 * 1024); // 1MB
            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
        }
        
        size_t final_memory = getCurrentMemoryUsage();
        size_t diff = final_memory > initial_memory ? 
                     final_memory - initial_memory : 0;
        
        std::cout << "Initial memory usage: " << initial_memory << " bytes\n";
        std::cout << "Final memory usage: " << final_memory << " bytes\n";
        std::cout << "Difference: " << diff << " bytes\n";
    }

    static void testConcurrentMemoryAccess() {
        std::cout << "\n4. Concurrent Memory Access Test\n";
        
        const int NUM_THREADS = 4;
        std::vector<std::thread> threads;
        std::atomic<bool> error_occurred{false};
        
        // Shared memory area
        auto shared_memory = std::make_unique<uint8_t[]>(1024 * 1024);
        
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&shared_memory, i, &error_occurred]() {
                try {
                    // Each thread writes to its own area
                    size_t offset = i * (1024 * 1024 / NUM_THREADS);
                    size_t size = 1024 * 1024 / NUM_THREADS;
                    
                    std::memset(shared_memory.get() + offset, i, size);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    
                    // Verify written data
                    for (size_t j = 0; j < size; ++j) {
                        if (shared_memory[offset + j] != i) {
                            error_occurred = true;
                            break;
                        }
                    }
                } catch (...) {
                    error_occurred = true;
                }
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        std::cout << "Concurrent access test: " 
                  << (error_occurred ? "Failed" : "Success") << "\n";
    }

    static void testMemoryBoundaries() {
        std::cout << "\n5. Memory Boundaries Test\n";
        
        try {
            // Try to allocate a large memory
            size_t huge_size = static_cast<size_t>(1024) * 1024 * 1024 * 10; // 10GB
            std::cout << "Trying to allocate 10GB memory...\n";
            
            auto huge_allocation = std::make_unique<uint8_t[]>(huge_size);
            std::cout << "Large memory allocation successful\n";
        } catch (const std::bad_alloc& e) {
            std::cout << "Expected memory allocation error: Too large size\n";
        }
        
        try {
            // Zero size allocation
            auto zero_allocation = std::make_unique<uint8_t[]>(0);
            std::cout << "Zero size allocation successful\n";
        } catch (const std::exception& e) {
            std::cout << "Zero size error: " << e.what() << "\n";
        }
    }

    // Helper functions
    static MemoryMetrics testAllocation(size_t size) {
        MemoryMetrics metrics{};
        
        auto start_memory = getCurrentMemoryUsage();
        auto data = std::make_unique<uint8_t[]>(size);
        auto peak_memory = getCurrentMemoryUsage();
        
        metrics.allocated_memory = size;
        metrics.peak_memory = peak_memory - start_memory;
        
        auto start = std::chrono::high_resolution_clock::now();
        QuantumResistantMemory::secureWipe(data.get(), size);
        auto end = std::chrono::high_resolution_clock::now();
        
        metrics.secure_cleanup_time = 
            std::chrono::duration<double, std::milli>(end - start).count();
        
        return metrics;
    }

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
                    return pages * getpagesize(); // sysconf instead of getpagesize()
                }
                fclose(statm);
            }
            return 0;
        #else
            return 0;
        #endif
    }
};

int main() {
    try {
        MemoryTest::runMemoryTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 