#include <skein3.h>
#include <iostream>
#include <iomanip>

void test_standard_hashing() {
    std::vector<uint8_t> message = {1, 2, 3, 4, 5};
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;

    auto hash = Skein3::hash(message, config);
    
    std::cout << "Hash: ";
    for (uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

void test_streaming_hash() {
    std::vector<uint8_t> chunk1 = {1, 2, 3};
    std::vector<uint8_t> chunk2 = {4, 5, 6};

    Skein3::StreamingHasher hasher;
    hasher.update(chunk1);
    hasher.update(chunk2);
    auto final_hash = hasher.finalize();

    std::cout << "Streaming Hash: ";
    for (uint8_t byte : final_hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

void test_ai_optimized_hash() {
    std::vector<uint8_t> message = {7, 8, 9};
    Skein3::Config config;
    config.neural_config.enable_neural_adaptation = true;
    config.neural_config.complexity_threshold = 0.85f;

    auto hash = Skein3::hash(message, config);

    std::cout << "AI-Optimized Hash: ";
    for (uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

int main() {
    test_standard_hashing();
    test_streaming_hash();
    test_ai_optimized_hash();
    return 0;
} 