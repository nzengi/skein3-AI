#ifndef AI_PROTECTION_H
#define AI_PROTECTION_H

#include <vector>
#include <cstdint>

class AIModelProtection {
public:
    // Model koruma fonksiyonu
    static std::vector<uint8_t> protectModel(
        const std::vector<float>& weights,
        const std::vector<uint8_t>& key
    );

    // Ağırlıkları güvenli şekilde güncelleme
    static bool updateWeightsSecurely(
        std::vector<float>& weights,
        const std::vector<uint8_t>& key
    );

private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t KEY_SIZE = 32;
};

#endif // AI_PROTECTION_H 