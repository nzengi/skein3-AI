#include "skein3.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

int main() {
    try {
        // Example message
        std::string message = "This is a C++ implementation of the Skein3 hash function.";
        std::vector<uint8_t> message_bytes(message.begin(), message.end());

        // Configure hash options
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_512;  // Use 512-bit output
        config.mode = Skein3::HashMode::STANDARD;   // Use standard mode

        // Compute hash using new API
        std::vector<uint8_t> hash = Skein3::hash(message_bytes, config);

        // Print hash value in hexadecimal format
        std::cout << "Skein3 Hash: ";
        for (const uint8_t byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Example of using different modes
        
        // Tree mode for parallel processing
        config.mode = Skein3::HashMode::TREE;
        config.tree_leaf_size = 1024;
        config.tree_fan_out = 4;
        std::vector<uint8_t> tree_hash = Skein3::tree_hash(message_bytes, config);
        
        // Streaming mode for large data
        Skein3::StreamingHasher hasher(config);
        hasher.update(message_bytes);
        std::vector<uint8_t> stream_hash = hasher.finalize();

        // Print results
        std::cout << "\nTree Mode Hash: ";
        for (const uint8_t byte : tree_hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        std::cout << std::endl;

        std::cout << "Stream Mode Hash: ";
        for (const uint8_t byte : stream_hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
