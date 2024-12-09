#ifndef LICENSE_H
#define LICENSE_H

#include <string>
#include <chrono>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <mutex>

class LicenseManager {
public:
    enum class LicenseType {
        BASIC_COMMERCIAL,
        ENTERPRISE,
        AI_SPECIFIC,          // Special license for AI companies
        BLOCKCHAIN_SPECIFIC,  // Special license for Blockchain companies
        UNLIMITED,            // Unlimited usage license
        NON_COMMERCIAL
    };

    enum class Feature {
        QUANTUM_RESISTANCE,
        PARALLEL_PROCESSING,
        GPU_ACCELERATION,
        BATCH_PROCESSING,
        MERKLE_TREE,
        ZERO_KNOWLEDGE,
        AI_OPTIMIZATION,
        BLOCKCHAIN_OPTIMIZATION
    };

    struct License {
        std::string key;
        std::string organization;
        LicenseType type;
        std::chrono::system_clock::time_point expiry;
        bool is_valid;
    };

    static LicenseManager& getInstance() {
        static LicenseManager instance;
        return instance;
    }

    bool validateLicense(const std::string& license_key);
    bool isCommercialUse() const;
    void setLicense(const std::string& license_key);
    const License& getCurrentLicense() const;

private:
    LicenseManager() = default;
    License current_license_;
    mutable std::mutex license_mutex_;
    
    // Custom hash implementation instead of OpenSSL
    std::vector<uint8_t> generateHash(const std::string& data);
    bool verifySignature(const std::string& license_key);
    void decodeLicenseKey(const std::string& license_key);
    
    // Custom hash function
    uint64_t customHash(const std::string& data) {
        uint64_t hash = 0x1234567890ABCDEF;
        for (char c : data) {
            hash ^= static_cast<uint64_t>(c);
            hash *= 0x100000001B3;
            hash ^= hash >> 32;
        }
        return hash;
    }
};

class LicenseException : public std::runtime_error {
public:
    explicit LicenseException(const char* message) 
        : std::runtime_error(message) {}
};

#endif // LICENSE_H 