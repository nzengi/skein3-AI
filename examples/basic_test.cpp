#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>

// Function declaration
void printHash(const std::vector<uint8_t>& hash);

void testBasicFunctionality() {
    try {
        // Test case 1: Simple string
        std::string test_message = "Hello Skein3!";
        std::vector<uint8_t> data(test_message.begin(), test_message.end());

        // Test case 2: Binary data
        std::vector<uint8_t> binary_data = {0xFF, 0x00, 0xAA, 0x55, 0x12, 0x34, 0x78, 0x9A};

        // Test case 3: Random data
        std::vector<uint8_t> random_data(1024); // 1KB
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        for(auto& byte : random_data) {
            byte = static_cast<uint8_t>(dis(gen));
        }

        // Test different configurations
        std::vector<Skein3::Config> configs;
        
        // Config 1: Basic 256-bit
        Skein3::Config config256;
        config256.size = Skein3::HashSize::HASH_256;
        configs.push_back(config256);

        // Config 2: Standard 512-bit
        Skein3::Config config512;
        config512.size = Skein3::HashSize::HASH_512;
        configs.push_back(config512);

        // Config 3: Quantum-resistant 1024-bit
        Skein3::Config config1024;
        config1024.size = Skein3::HashSize::HASH_1024;
        configs.push_back(config1024);

        // Test each configuration with each data set
        for(const auto& config : configs) {
            std::cout << "\nTesting " << static_cast<int>(config.size) << "-bit hash:\n";
            
            // Test string data
            auto hash1 = Skein3::hash(data, config);
            std::cout << "String data hash (" << hash1.size() << " bytes): ";
            printHash(hash1);

            // Test binary data
            auto hash2 = Skein3::hash(binary_data, config);
            std::cout << "Binary data hash (" << hash2.size() << " bytes): ";
            printHash(hash2);

            // Test random data
            auto hash3 = Skein3::hash(random_data, config);
            std::cout << "Random data hash (" << hash3.size() << " bytes): ";
            printHash(hash3);

            // Verify different inputs produce different hashes
            assert(hash1 != hash2 && hash2 != hash3 && hash1 != hash3);
            
            // Verify consistency
            auto verify_hash = Skein3::hash(data, config);
            assert(verify_hash == hash1);
        }

        std::cout << "\nAll basic functionality tests passed!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

// Function implementation
void printHash(const std::vector<uint8_t>& hash) {
    for (uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;
}

int main() {
    try {
        std::cout << "Starting basic functionality tests...\n";
        testBasicFunctionality();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
} 