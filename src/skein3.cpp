#include "skein3.h"
#include "threefish3.h"
#include <cstring>

void UBI(std::array<uint64_t, Threefish3::NUM_WORDS>& G, const std::array<uint64_t, Threefish3::NUM_WORDS>& block, 
         const std::array<uint64_t, 3>& tweak) {
    std::array<uint64_t, Threefish3::NUM_WORDS> ciphertext;
    Threefish3 cipher(G, tweak);
    cipher.encrypt(block, ciphertext);

    for (size_t i = 0; i < Threefish3::NUM_WORDS; ++i) {
        G[i] ^= ciphertext[i];
    }
}

std::vector<uint8_t> skein3_hash(const std::vector<uint8_t>& message, size_t hash_size) {
    const size_t block_size = Threefish3::BLOCK_SIZE;
    const size_t num_words = Threefish3::NUM_WORDS;

    // Başlangıç durumu (IV)
    std::array<uint64_t, num_words> G = {};
    std::array<uint64_t, 3> tweak = {0, 0, 0};

    // Mesajı bloklara böl
    size_t num_blocks = (message.size() + block_size - 1) / block_size;
    std::vector<uint8_t> padded_message = message;
    padded_message.resize(num_blocks * block_size, 0);

    // UBI her blok için çalıştır
    for (size_t i = 0; i < num_blocks; ++i) {
        std::array<uint64_t, num_words> block = {};
        std::memcpy(block.data(), &padded_message[i * block_size], block_size);
        tweak[0] = i;
        UBI(G, block, tweak);
    }

    // Çıktıyı oluştur
    std::vector<uint8_t> hash(hash_size);
    std::memcpy(hash.data(), G.data(), hash_size);
    return hash;
}
