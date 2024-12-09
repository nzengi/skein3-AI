#include "license.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include "../include/base64.h"
#include <mutex>

bool LicenseManager::validateLicense(const std::string& license_key) {
    try {
        std::cout << "Debug: Validating license: " << license_key << "\n";
        std::cout << "Debug: Length: " << license_key.length() << "\n";
        std::cout << "Debug: Prefix: " << license_key.substr(0, 7) << "\n";
        std::cout << "Debug: Dash position: " << license_key[14] << "\n";
        
        // Format kontrolü
        if (license_key.length() != 22) {
            std::cout << "Debug: Invalid length\n";
            return false;
        }
        
        if (license_key.substr(0, 7) != "SKEIN3-") {
            std::cout << "Debug: Invalid prefix\n";
            return false;
        }
        
        if (license_key[14] != '-') {
            std::cout << "Debug: Invalid dash position\n";
            return false;
        }

        // Kullanıcı ID'si ve doğrulama kodunu ayır
        std::string user_id = license_key.substr(7, 7);
        std::string validation = license_key.substr(15, 7);

        std::cout << "Debug: User ID: " << user_id << ", Validation: " << validation << "\n";  // Debug mesajı

        // Doğrulama kodunu hesapla ve karşılaştır
        std::string expected_validation = generateValidationCode(user_id);
        
        std::cout << "Debug: Expected validation: " << expected_validation << "\n";  // Debug mesajı
        
        if (validation != expected_validation) {
            std::cout << "Debug: Validation mismatch\n";  // Debug mesajı
            return false;
        }

        current_license_.is_valid = true;
        current_license_.type = LicenseType::BASIC_COMMERCIAL;
        current_license_.expiry = std::chrono::system_clock::now() + std::chrono::hours(24*365);
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Debug: Exception: " << e.what() << "\n";  // Debug mesajı
        current_license_.is_valid = false;
        return false;
    }
}

std::string LicenseManager::generateValidationCode(const std::string& user_id) {
    // Özel bir algoritma ile doğrulama kodu üret
    uint64_t hash = 0x1505;
    for (char c : user_id) {
        hash = ((hash << 5) + hash) + c;
    }
    
    // Son 7 karakteri al ve büyük harfe çevir
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(7) << (hash % 0x10000000);
    return ss.str();
}

bool LicenseManager::isCommercialUse() const {
    std::lock_guard<std::mutex> lock(license_mutex_);
    return current_license_.type != LicenseType::NON_COMMERCIAL && 
           current_license_.is_valid;
}

void LicenseManager::setLicense(const std::string& license_key) {
    try {
        if (!validateLicense(license_key)) {
            std::cout << "Debug: License validation failed\n";  // Debug mesajı
            current_license_.is_valid = false;
        }
    } catch (const std::exception& e) {
        std::cout << "Debug: " << e.what() << "\n";  // Debug mesajı
        current_license_.is_valid = false;
    }
}

bool LicenseManager::verifySignature(const std::string& license_key) {
    return license_key.length() >= 32;
}

void LicenseManager::decodeLicenseKey(const std::string& license_key) {
    current_license_.key = license_key;
    current_license_.type = LicenseType::BASIC_COMMERCIAL;
    current_license_.organization = "Example Org";
    current_license_.expiry = std::chrono::system_clock::now() + std::chrono::hours(24*365);
    current_license_.is_valid = true;
}

const LicenseManager::License& LicenseManager::getCurrentLicense() const {
    std::lock_guard<std::mutex> lock(license_mutex_);
    return current_license_;
}
