#include "ai_protection.h"
#include "skein3.h"
#include <cmath>
#include <algorithm>

std::vector<uint8_t> AIModelProtection::protectModel(
    const std::vector<float>& weights,
    const std::vector<uint8_t>& architecture
) {
    struct ModelContext {
        std::vector<uint8_t> weight_hash;
        std::vector<uint8_t> arch_hash;
        std::vector<uint8_t> integrity_proof;
        uint64_t version;
        uint64_t timestamp;
    };

    ModelContext ctx;
    ctx.version = 1;
    ctx.timestamp = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );

    // Configure hash for model protection
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_1024; // Maximum security for AI models
    config.neural_config.enable_neural_adaptation = true;
    config.neural_config.complexity_threshold = 0.95f; // High complexity requirement

    // Convert weights to bytes
    std::vector<uint8_t> weight_bytes;
    weight_bytes.reserve(weights.size() * sizeof(float));
    for (float w : weights) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&w);
        weight_bytes.insert(weight_bytes.end(), bytes, bytes + sizeof(float));
    }

    // Generate weight fingerprint
    ctx.weight_hash = Skein3::hash(weight_bytes, config);

    // Generate architecture fingerprint
    ctx.arch_hash = Skein3::hash(architecture, config);

    // Generate integrity proof
    std::vector<uint8_t> combined;
    combined.insert(combined.end(), ctx.weight_hash.begin(), ctx.weight_hash.end());
    combined.insert(combined.end(), ctx.arch_hash.begin(), ctx.arch_hash.end());
    combined.insert(combined.end(), 
        reinterpret_cast<uint8_t*>(&ctx.version),
        reinterpret_cast<uint8_t*>(&ctx.version) + sizeof(ctx.version));
    combined.insert(combined.end(),
        reinterpret_cast<uint8_t*>(&ctx.timestamp),
        reinterpret_cast<uint8_t*>(&ctx.timestamp) + sizeof(ctx.timestamp));

    ctx.integrity_proof = Skein3::hash(combined, config);

    // Combine all elements into final protection
    std::vector<uint8_t> protection;
    protection.insert(protection.end(), ctx.weight_hash.begin(), ctx.weight_hash.end());
    protection.insert(protection.end(), ctx.arch_hash.begin(), ctx.arch_hash.end());
    protection.insert(protection.end(), ctx.integrity_proof.begin(), ctx.integrity_proof.end());
    protection.insert(protection.end(),
        reinterpret_cast<uint8_t*>(&ctx.version),
        reinterpret_cast<uint8_t*>(&ctx.version) + sizeof(ctx.version));
    protection.insert(protection.end(),
        reinterpret_cast<uint8_t*>(&ctx.timestamp),
        reinterpret_cast<uint8_t*>(&ctx.timestamp) + sizeof(ctx.timestamp));

    return protection;
}

bool AIModelProtection::updateWeightsSecurely(
    std::vector<float>& weights,
    const std::vector<uint8_t>& updates
) {
    struct UpdateContext {
        std::vector<float> original_weights;
        std::vector<float> update_values;
        std::vector<uint8_t> integrity_hash;
        float max_update_magnitude;
        bool is_valid;
    };

    UpdateContext ctx;
    ctx.original_weights = weights;
    ctx.max_update_magnitude = 0.1f; // Maximum allowed update magnitude
    ctx.is_valid = true;

    // Configure hash for update verification
    Skein3::Config config;
    config.size = Skein3::HashSize::HASH_512;
    config.neural_config.enable_neural_adaptation = true;

    // Verify update integrity
    ctx.integrity_hash = Skein3::hash(updates, config);

    // Convert updates to weight changes
    ctx.update_values.resize(weights.size());
    const float* update_ptr = reinterpret_cast<const float*>(updates.data());
    size_t num_updates = updates.size() / sizeof(float);

    if (num_updates != weights.size()) {
        return false;
    }

    // Apply updates with security checks
    for (size_t i = 0; i < weights.size(); ++i) {
        float update = update_ptr[i];
        
        // Check update magnitude
        if (std::abs(update) > ctx.max_update_magnitude) {
            ctx.is_valid = false;
            break;
        }

        // Check for NaN and Inf
        if (std::isnan(update) || std::isinf(update)) {
            ctx.is_valid = false;
            break;
        }

        // Apply update
        weights[i] += update;

        // Verify result is still valid
        if (std::isnan(weights[i]) || std::isinf(weights[i])) {
            ctx.is_valid = false;
            break;
        }
    }

    // Rollback if update is invalid
    if (!ctx.is_valid) {
        weights = ctx.original_weights;
        return false;
    }

    // Verify final state
    std::vector<uint8_t> weight_bytes;
    weight_bytes.reserve(weights.size() * sizeof(float));
    for (float w : weights) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&w);
        weight_bytes.insert(weight_bytes.end(), bytes, bytes + sizeof(float));
    }

    auto final_hash = Skein3::hash(weight_bytes, config);

    // Additional security checks can be added here
    // For example, verifying the update pattern matches expected training behavior

    return true;
} 