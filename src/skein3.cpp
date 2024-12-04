#include "skein3.h"
#include "threefish3.h"
#include <cstring>

void UBI(std::array<uint64_t, Threefish3::NUM_WORDS>& G, const std::array<uint64_t, Threefish3::NUM_WORDS>& block, 
         const std::array<uint64_t, 3>& tweak) {
    std::array<uint64_t, Threefish3::NUM_WORDS> ciphertext;
    
    // Type field constants
    const uint64_t TYPE_MSG = 48;            // Message type
    const uint64_t TYPE_OUT = 63;            // Output type
    const uint64_t FIRST_BLOCK = 1ULL << 62;
    const uint64_t FINAL_BLOCK = 1ULL << 63;
    
    // Update tweak with type information
    std::array<uint64_t, 3> block_tweak = tweak;
    block_tweak[0] |= TYPE_MSG;
    
    Threefish3 cipher(G, block_tweak);
    cipher.encrypt(block, ciphertext);
    
    for (size_t i = 0; i < Threefish3::NUM_WORDS; ++i) {
        G[i] ^= block[i] ^ ciphertext[i];
    }
}

std::vector<uint8_t> skein3_hash(const std::vector<uint8_t>& message, size_t hash_size) {
    const size_t block_size = Threefish3::BLOCK_SIZE;
    const size_t num_words = Threefish3::NUM_WORDS;

    // Configuration block
    std::array<uint64_t, num_words> config = {};
    config[0] = 0x133414853; // Schema identifier
    config[1] = hash_size * 8; // Output size in bits
    
    // Initial state with configuration
    std::array<uint64_t, num_words> G = {};
    std::array<uint64_t, 3> config_tweak = {0, 1ULL << 62, 0}; // First block flag
    UBI(G, config, config_tweak);

    // Process message blocks
    size_t num_blocks = (message.size() + block_size - 1) / block_size;
    std::vector<uint8_t> padded_message = message;
    padded_message.resize(num_blocks * block_size, 0);

    for (size_t i = 0; i < num_blocks; ++i) {
        std::array<uint64_t, num_words> block = {};
        std::memcpy(block.data(), &padded_message[i * block_size], block_size);
        
        std::array<uint64_t, 3> tweak = {
            static_cast<uint64_t>(i * block_size), // Position
            (i == 0) ? 1ULL << 62 : 0,            // First block flag
            (i == num_blocks - 1) ? 1ULL << 63 : 0 // Final block flag
        };
        
        UBI(G, block, tweak);
    }

    // Output transformation
    std::array<uint64_t, num_words> output_block = {};
    std::array<uint64_t, 3> output_tweak = {0, 0, 1ULL << 63}; // Final block flag
    UBI(G, output_block, output_tweak);

    // Generate final hash
    std::vector<uint8_t> hash(hash_size);
    std::memcpy(hash.data(), G.data(), hash_size);
    return hash;
}
