#include "threefish3.h"
#include <cstddef>  // size_t için
#include <cstdint>

Threefish3::Threefish3(const std::array<uint64_t, NUM_WORDS>& key, const std::array<uint64_t, 3>& tweak)
    : key_(key), tweak_(tweak) {}

void Threefish3::encrypt(const std::array<uint64_t, NUM_WORDS>& plaintext, std::array<uint64_t, NUM_WORDS>& ciphertext) {
    std::array<uint64_t, NUM_WORDS> data = plaintext;
    
    // Key schedule initialization
    std::array<uint64_t, NUM_WORDS + 1> extended_key;
    for (size_t i = 0; i < NUM_WORDS; i++) {
        extended_key[i] = key_[i];
    }
    
    uint64_t key_parity = 0x1BD11BDAA9FC1A22ULL;
    for (size_t i = 0; i < NUM_WORDS; i++) {
        key_parity ^= key_[i];
    }
    extended_key[NUM_WORDS] = key_parity;
    
    // Main encryption loop
    for (size_t d = 0; d < NUM_ROUNDS / 4; d++) {
        // Add round key
        for (size_t i = 0; i < NUM_WORDS; i++) {
            data[i] += extended_key[(d + i) % (NUM_WORDS + 1)];
        }
        data[0] += tweak_[d % 3];
        
        // Mix function
        for (size_t j = 0; j < 4; j++) {
            // Mix and permute
            for (size_t i = 0; i < NUM_WORDS/2; i++) {
                size_t idx1 = 2 * i;
                size_t idx2 = 2 * i + 1;
                
                uint64_t x0 = data[idx1];
                uint64_t x1 = data[idx2];
                
                x0 += x1;
                x1 = ((x1 << 49) | (x1 >> 15)) ^ x0;
                
                data[idx1] = x0;
                data[idx2] = x1;
            }
            
            // Word permutation
            std::array<uint64_t, NUM_WORDS> temp = data;
            for (size_t i = 0; i < NUM_WORDS; i++) {
                data[i] = temp[(i + 2) % NUM_WORDS];
            }
        }
    }
    
    ciphertext = data;
}
