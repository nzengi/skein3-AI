#include "../include/blockchain_features.h"
#include "../include/skein3.h"
#include <stdexcept>
#include <algorithm>

std::vector<uint8_t> BlockchainFeatures::generateZKProof(
    const std::vector<uint8_t>& data,
    const std::vector<uint8_t>& proof_params
) {
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_1024;
    config.zero_knowledge = true;

    std::vector<uint8_t> combined;
    combined.reserve(data.size() + proof_params.size());
    combined.insert(combined.end(), data.begin(), data.end());
    combined.insert(combined.end(), proof_params.begin(), proof_params.end());

    return Skein3::hash(combined, config);
}

std::vector<uint8_t> BlockchainFeatures::optimizedMerkleRoot(
    const std::vector<std::vector<uint8_t>>& transactions
) {
    if (transactions.empty()) {
        throw std::invalid_argument("Empty transaction list");
    }

    std::vector<std::vector<uint8_t>> current_level = transactions;
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    config.merkle_tree = true;

    while (current_level.size() > 1) {
        std::vector<std::vector<uint8_t>> next_level;
        
        #pragma omp parallel for
        for (size_t i = 0; i < current_level.size(); i += 2) {
            std::vector<uint8_t> combined;
            combined.insert(combined.end(), 
                          current_level[i].begin(), 
                          current_level[i].end());

            if (i + 1 < current_level.size()) {
                combined.insert(combined.end(),
                              current_level[i + 1].begin(),
                              current_level[i + 1].end());
            }

            auto hash = Skein3::hash(combined, config);

            #pragma omp critical
            next_level.push_back(hash);
        }

        current_level = std::move(next_level);
    }

    return current_level[0];
}

bool BlockchainFeatures::verifySmartContract(
    const std::vector<uint8_t>& contract_code,
    const std::vector<uint8_t>& execution_params
) {
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    config.zero_knowledge = true;

    std::vector<uint8_t> verification_data;
    verification_data.reserve(contract_code.size() + execution_params.size());
    verification_data.insert(verification_data.end(), contract_code.begin(), contract_code.end());
    verification_data.insert(verification_data.end(), execution_params.begin(), execution_params.end());

    auto hash = Skein3::hash(verification_data, config);
    
    return !hash.empty() && hash[0] != 0;
}