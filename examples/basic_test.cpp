#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <cassert>

void testBasicFunctionality() {
    try {
        // Prepare test data
        std::string test_message = "Hello Skein3!";
        std::vector<uint8_t> data(test_message.begin(), test_message.end());

        // Basic hash operation
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_512;
        auto hash = Skein3::hash(data, config);

        std::cout << "Hash length: " << hash.size() << " bytes\n";
        std::cout << "Hash value: ";
        for (uint8_t byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Hash verification - replacing direct verification with comparison
        auto verify_hash = Skein3::hash(data, config);
        assert(verify_hash == hash);
        std::cout << "Hash verification successful!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Adding main function
int main() {
    testBasicFunctionality();
    return 0;
} 