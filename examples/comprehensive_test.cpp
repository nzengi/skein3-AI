#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cassert>

class ComprehensiveTest {
public:
    static void runAllTests() {
        std::cout << "Starting comprehensive test...\n\n";

        testEdgeCases();
        testSpecialInputs();
        testLargeData();
        testRandomData();
        testConsistency();
    }

private:
    static void testEdgeCases() {
        std::cout << "1. Edge Cases Test\n";

        {
            std::vector<uint8_t> empty_data;
            Skein3::Config config;
            auto hash = Skein3::hash(empty_data, config);
            std::cout << "Empty data hash length: " << hash.size() << " bytes\n";
        }

        {
            std::vector<uint8_t> single_byte{0xFF};
            Skein3::Config config;
            auto hash = Skein3::hash(single_byte, config);
            std::cout << "Single byte hash length: " << hash.size() << " bytes\n";
        }

        try {
            std::vector<uint8_t> huge_data(1ULL << 31);
            Skein3::Config config;
            auto hash = Skein3::hash(huge_data, config);
            std::cout << "Large data hashed\n";
        } catch (const std::exception& e) {
            std::cout << "Expected error: " << e.what() << "\n";
        }
    }

    static void testSpecialInputs() {
        std::cout << "\n2. Special Input Test\n";

        {
            std::vector<uint8_t> all_zeros(1024, 0x00);
            Skein3::Config config;
            auto hash = Skein3::hash(all_zeros, config);
            printHash("All zeros hash", hash);
        }

        {
            std::vector<uint8_t> all_ones(1024, 0xFF);
            Skein3::Config config;
            auto hash = Skein3::hash(all_ones, config);
            printHash("All ones hash", hash);
        }

        {
            std::vector<uint8_t> pattern;
            for (int i = 0; i < 1024; ++i) {
                pattern.push_back(i % 2 ? 0xFF : 0x00);
            }
            Skein3::Config config;
            auto hash = Skein3::hash(pattern, config);
            printHash("Alternating pattern hash", hash);
        }

        {
            std::string utf8_str = "Hello, World! 🌍";
            std::vector<uint8_t> utf8_data(utf8_str.begin(), utf8_str.end());
            Skein3::Config config;
            auto hash = Skein3::hash(utf8_data, config);
            printHash("UTF-8 hash", hash);
        }
    }

    static void testLargeData() {
        std::cout << "\n3. Large Data Test\n";

        std::vector<size_t> sizes = {
            1024 * 1024,
            10 * 1024 * 1024,
            100 * 1024 * 1024
        };

        for (size_t size : sizes) {
            std::vector<uint8_t> data(size);
            fillRandomData(data);

            auto start = std::chrono::high_resolution_clock::now();
            
            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start
            ).count();

            std::cout << "Size: " << (size / 1024.0 / 1024.0) << " MB, "
                     << "Time: " << duration << " ms\n";
        }
    }

    static void testRandomData() {
        std::cout << "\n4. Random Data Test\n";

        const int NUM_TESTS = 1000;
        std::unordered_map<std::string, int> hash_counts;

        for (int i = 0; i < NUM_TESTS; ++i) {
            size_t size = 1024 + (rand() % (1024 * 1024));
            std::vector<uint8_t> data(size);
            fillRandomData(data);

            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
            
            std::string hash_str(hash.begin(), hash.end());
            hash_counts[hash_str]++;
        }

        bool collision_found = false;
        for (const auto& [hash, count] : hash_counts) {
            if (count > 1) {
                std::cout << "Collision found! Hash repeat count: " 
                         << count << "\n";
                collision_found = true;
            }
        }

        if (!collision_found) {
            std::cout << "No collisions found (" << NUM_TESTS << " tests)\n";
        }
    }

    static void testConsistency() {
        std::cout << "\n5. Consistency Test\n";

        std::vector<uint8_t> data(1024);
        fillRandomData(data);

        std::vector<Skein3::Config> configs;
        
        configs.push_back(Skein3::Config{});
        
        Skein3::Config quantum_config;
        quantum_config.size = Skein3::HashSize::HASH_1024;
        quantum_config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;
        configs.push_back(quantum_config);
        
        Skein3::Config ai_config;
        Skein3::optimize_for_ai(ai_config);
        configs.push_back(ai_config);

        for (const auto& config : configs) {
            std::vector<std::vector<uint8_t>> hashes;
            
            for (int i = 0; i < 5; ++i) {
                auto hash = Skein3::hash(data, config);
                hashes.push_back(hash);
            }

            bool consistent = true;
            for (size_t i = 1; i < hashes.size(); ++i) {
                if (hashes[i] != hashes[0]) {
                    consistent = false;
                    break;
                }
            }

            std::cout << "Config #" << (&config - &configs[0]) + 1 
                     << " consistency: " 
                     << (consistent ? "Success" : "Failed") << "\n";
        }
    }

    static void fillRandomData(std::vector<uint8_t>& data) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        for (auto& byte : data) {
            byte = static_cast<uint8_t>(dis(gen));
        }
    }

    static void printHash(const std::string& label, 
                         const std::vector<uint8_t>& hash) {
        std::cout << label << ": ";
        for (uint8_t byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte);
        }
        std::cout << std::dec << "\n";
    }
};

int main() {
    try {
        ComprehensiveTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 