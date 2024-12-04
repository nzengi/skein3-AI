#ifndef THREEFISH3_H
#define THREEFISH3_H

#include <array>
#include <cstdint>

// Threefish3 sınıfı
class Threefish3 {
public:
    static const size_t BLOCK_SIZE = 128; // 128 byte (1024 bit)
    static const size_t NUM_WORDS = BLOCK_SIZE / 8;

    Threefish3(const std::array<uint64_t, NUM_WORDS>& key, const std::array<uint64_t, 3>& tweak);
    void encrypt(const std::array<uint64_t, NUM_WORDS>& plaintext, std::array<uint64_t, NUM_WORDS>& ciphertext);

private:
    static const size_t NUM_ROUNDS = 72; // Şifreleme tur sayısı
    std::array<uint64_t, NUM_WORDS> key_;
    std::array<uint64_t, 3> tweak_;
};

#endif // THREEFISH3_H
