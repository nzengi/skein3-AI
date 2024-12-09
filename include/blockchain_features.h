#ifndef BLOCKCHAIN_FEATURES_H
#define BLOCKCHAIN_FEATURES_H

#include <vector>
#include <cstdint>
#include <string>

struct BlockchainFeatures {
    // Zero-knowledge proof oluşturma
    static std::vector<uint8_t> generateZKProof(
        const std::vector<uint8_t>& data,
        const std::vector<uint8_t>& proof_params
    );

    // Optimize edilmiş Merkle kök hesaplama
    static std::vector<uint8_t> optimizedMerkleRoot(
        const std::vector<std::vector<uint8_t>>& transactions
    );

    // Akıllı kontrat doğrulama
    static bool verifySmartContract(
        const std::vector<uint8_t>& contract_code,
        const std::vector<uint8_t>& execution_params
    );

private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t TREE_HEIGHT = 32;
};

#endif // BLOCKCHAIN_FEATURES_H 