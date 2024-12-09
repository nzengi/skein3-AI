#include "license.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include "../include/base64.h"
#include <mutex>

bool LicenseManager::validateLicense(const std::string& license_key) {
    try {
        if (!verifySignature(license_key)) {
            throw LicenseException("Invalid license signature");
        }

        decodeLicenseKey(license_key);

        // Check expiry
        if (current_license_.expiry < std::chrono::system_clock::now()) {
            throw LicenseException("License has expired");
        }

        current_license_.is_valid = true;
        return true;
    }
    catch (const std::exception&) {
        current_license_.is_valid = false;
        return false;
    }
}

bool LicenseManager::isCommercialUse() const {
    std::lock_guard<std::mutex> lock(license_mutex_);
    return current_license_.type != LicenseType::NON_COMMERCIAL && 
           current_license_.is_valid;
}

void LicenseManager::setLicense(const std::string& license_key) {
    if (!validateLicense(license_key)) {
        throw LicenseException("Invalid license key");
    }
}

bool LicenseManager::verifySignature(const std::string& license_key) {
    // Basit bir doğrulama örneği
    return license_key.length() >= 32;  // En az 32 karakter olmalı
}

void LicenseManager::decodeLicenseKey(const std::string& license_key) {
    // Lisans anahtarını decode et
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

// Diğer private fonksiyonlar... 