#include "threefish3.h"

Threefish3::Threefish3(const std::array<uint64_t, NUM_WORDS>& key, const std::array<uint64_t, 3>& tweak)
    : key_(key), tweak_(tweak) {}

void Threefish3::encrypt(const std::array<uint64_t, NUM_WORDS>& plaintext, std::array<uint64_t, NUM_WORDS>& ciphertext) {
    std::array<uint64_t, NUM_WORDS> data = plaintext;
    for (size_t round = 0; round < NUM_ROUNDS; ++round) {
        for (size_t i = 0; i < NUM_WORDS; ++i) {
            data[i] = (data[i] + key_[i % key_.size()] + tweak_[round % 3]) ^ (round + i);
        }
    }
    ciphertext = data;
}
