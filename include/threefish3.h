#ifndef THREEFISH3_H
#define THREEFISH3_H

#include <array>
#include <cstdint>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <immintrin.h>
#include "thread_pool.h"

class Threefish3 {
public:
    // Sabitler
    static const size_t BLOCK_SIZE = 256;
    static const size_t NUM_WORDS = BLOCK_SIZE / 8;
    static const size_t NUM_ROUNDS = 72;
    static const size_t MIX_ROUNDS = 8;
    static constexpr uint64_t QUANTUM_CONSTANT = 0x1BD11BDAA9FC1A22ULL;

    enum class SecurityMode {
        STANDARD,
        ENHANCED,
        QUANTUM_RESISTANT,
        QUANTUM
    };

    enum class OperationMode {
        STANDARD,
        CASCADE,
        ADAPTIVE
    };

    Threefish3(const std::array<uint64_t, NUM_WORDS>& state,
               const std::array<uint64_t, 3>& tweak,
               SecurityMode mode);
    
    ~Threefish3();

    void encrypt(const std::array<uint64_t, NUM_WORDS>& plaintext,
                std::array<uint64_t, NUM_WORDS>& ciphertext);

    void decrypt(const std::array<uint64_t, NUM_WORDS>& ciphertext,
                std::array<uint64_t, NUM_WORDS>& plaintext);

    void simd_mix_function(__m256i& block, int round);
    void simd_permute_words(std::array<__m256i, NUM_WORDS/4>& data);
    void simd_inverse_mix_function(__m256i& block, int round);

    void parallel_encrypt(const std::vector<uint8_t>& input,
                        std::vector<uint8_t>& output,
                        size_t num_threads);
    void parallel_decrypt(const std::vector<uint8_t>& input,
                        std::vector<uint8_t>& output,
                        size_t num_threads);

    void benchmark(size_t data_size);
    void generate_dynamic_tables();
    void set_operation_mode(OperationMode mode);
    void cascade_encrypt(std::vector<uint64_t>& data);
    void adaptive_encrypt(std::vector<uint64_t>& data);
    void quantum_resistant_transform(std::vector<uint64_t>& data);
    void avalanche_mix(std::vector<uint64_t>& data);
    uint64_t quantum_mix(uint64_t value, size_t iteration);
    void apply_lattice_transformation(std::vector<uint64_t>& data);
    void process_chunk(const uint8_t* input, uint8_t* output, size_t chunk_size);
    void encrypt_chunk(const uint8_t* input, uint8_t* output,
                      size_t start_block, size_t num_blocks);
    void decrypt_chunk(const uint8_t* input, uint8_t* output,
                      size_t start_block, size_t num_blocks);

private:
    std::array<uint64_t, NUM_WORDS> state_;
    std::array<uint64_t, NUM_WORDS> key_;
    std::array<uint64_t, 3> tweak_;
    SecurityMode mode_;
    OperationMode op_mode_ = OperationMode::STANDARD;
    size_t block_size_ = BLOCK_SIZE;
    std::vector<uint8_t> permutation_table_;
    std::vector<uint8_t> inverse_permutation_table_;

    void quantum_init();
    void mix_function(uint64_t& x0, uint64_t& x1, int round);
    void permute_words(std::array<uint64_t, NUM_WORDS>& data);
    void inverse_mix_function(uint64_t& x0, uint64_t& x1, int round);
};

#endif // THREEFISH3_H
