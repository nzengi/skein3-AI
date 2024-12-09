#include "skein3.h"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>

void test_basic_hash() {
    // Test messages
    std::string message1 = "Test message 1";
    std::string message2 = "Test message 2";

    // Configure hash options
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    config.mode = Skein3::HashMode::STANDARD;

    // Compute hashes
    auto hash1 = Skein3::hash(std::vector<uint8_t>(message1.begin(), message1.end()), config);
    auto hash2 = Skein3::hash(std::vector<uint8_t>(message2.begin(), message2.end()), config);

    // Verify different messages produce different hashes
    assert(hash1 != hash2);

    // Verify hash length
    assert(hash1.size() == 64); // 512 bits = 64 bytes
    assert(hash2.size() == 64);
}

void test_empty_message() {
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    
    auto empty_hash = Skein3::hash(std::vector<uint8_t>(), config);
    assert(empty_hash.size() == 64);
}

void test_long_message() {
    // Create a 1MB test message
    std::vector<uint8_t> long_message(1000000, 'a');
    
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    
    auto long_hash = Skein3::hash(long_message, config);
    assert(long_hash.size() == 64);
}

void test_different_sizes() {
    std::vector<uint8_t> data{1, 2, 3};
    
    // Test different hash sizes
    Skein3::Config config256;
    config256.size = Skein3::HashSize::HASH_256;
    auto hash_256 = Skein3::hash(data, config256);
    assert(hash_256.size() == 32); // 256 bits = 32 bytes
    
    Skein3::Config config512;
    config512.size = Skein3::HashSize::HASH_512;
    auto hash_512 = Skein3::hash(data, config512);
    assert(hash_512.size() == 64); // 512 bits = 64 bytes
    
    Skein3::Config config1024;
    config1024.size = Skein3::HashSize::HASH_1024;
    auto hash_1024 = Skein3::hash(data, config1024);
    assert(hash_1024.size() == 128); // 1024 bits = 128 bytes
}

void test_tree_mode() {
    std::vector<uint8_t> data(100000, 'x'); // 100KB test data
    
    Skein3::Config config;
    config.mode = Skein3::HashMode::TREE;
    config.tree_leaf_size = 1024;
    config.tree_fan_out = 4;
    
    auto tree_hash = Skein3::tree_hash(data, config);
    assert(tree_hash.size() == 64);
}

void test_streaming_mode() {
    std::string message = "This is a test message for streaming mode";
    std::vector<uint8_t> data(message.begin(), message.end());
    
    Skein3::Config config;
    Skein3::StreamingHasher hasher(config);
    
    // Process data in chunks
    size_t chunk_size = 10;
    for (size_t i = 0; i < data.size(); i += chunk_size) {
        size_t size = std::min(chunk_size, data.size() - i);
        std::vector<uint8_t> chunk(data.begin() + i, data.begin() + i + size);
        hasher.update(chunk);
    }
    
    auto stream_hash = hasher.finalize();
    assert(stream_hash.size() == 64);
}

void test_mac() {
    // Test with a standard message and key
    std::string message1 = "Test message for MAC";
    std::vector<uint8_t> data1(message1.begin(), message1.end());
    std::vector<uint8_t> key1(32, 0x42); // 32-byte test key
    
    Skein3::Config config;
    auto mac_value1 = Skein3::mac(data1, key1, config);
    assert(mac_value1.size() == 64);

    // Test with a different message
    std::string message2 = "Another test message for MAC";
    std::vector<uint8_t> data2(message2.begin(), message2.end());
    std::vector<uint8_t> key2(32, 0x43); // Different key
    
    auto mac_value2 = Skein3::mac(data2, key2, config);
    assert(mac_value2.size() == 64);

    // Verify that different messages produce different MACs
    assert(mac_value1 != mac_value2);
}

void print_hash(const std::string& label, const std::vector<uint8_t>& hash) {
    std::cout << label << ": ";
    for (uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << std::endl;
}

int main() {
    try {
        test_basic_hash();
        test_empty_message();
        test_long_message();
        test_different_sizes();
        test_tree_mode();
        test_streaming_mode();
        test_mac();

        std::cout << "All tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
