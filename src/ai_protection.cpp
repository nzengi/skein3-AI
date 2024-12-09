#include "../include/ai_protection.h"
#include "../include/skein3.h"
#include <cstring>
#include <stdexcept>

std::vector<uint8_t> AIModelProtection::protectModel(
    const std::vector<float>& weights,
    const std::vector<uint8_t>& key
) {
    if (key.size() != KEY_SIZE) {
        throw std::invalid_argument("Invalid key size");
    }

    std::vector<uint8_t> weight_bytes;
    weight_bytes.reserve(weights.size() * sizeof(float));
    
    for (const float& weight : weights) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&weight);
        weight_bytes.insert(weight_bytes.end(), bytes, bytes + sizeof(float));
    }

    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;
    config.secure_memory_wipe = true;

    std::vector<uint8_t> protected_data;
    protected_data.reserve(weight_bytes.size() + KEY_SIZE);

    for (size_t i = 0; i < weight_bytes.size(); i += BLOCK_SIZE) {
        size_t block_size = std::min(BLOCK_SIZE, weight_bytes.size() - i);
        std::vector<uint8_t> block(weight_bytes.begin() + i, 
                                  weight_bytes.begin() + i + block_size);

        block.insert(block.end(), key.begin(), key.end());

        auto hash = Skein3::hash(block, config);
        protected_data.insert(protected_data.end(), hash.begin(), hash.end());
    }

    return protected_data;
}

bool AIModelProtection::updateWeightsSecurely(
    std::vector<float>& weights,
    const std::vector<uint8_t>& key
) {
    if (key.size() != KEY_SIZE) {
        return false;
    }

    try {
        Skein3::Config config;
        config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;
        config.secure_memory_wipe = true;
        config.memory_protection_rounds = 3;

        auto secure_weights = Skein3::allocateSecureMemory<float>(
            weights.size(), 
            config
        );

        std::memcpy(secure_weights.get(), weights.data(), 
                   weights.size() * sizeof(float));

        for (size_t i = 0; i < weights.size(); i++) {
            secure_weights[i] *= 1.01f;
        }

        std::memcpy(weights.data(), secure_weights.get(), 
                   weights.size() * sizeof(float));

        return true;
    } catch (const std::exception&) {
        return false;
    }
}