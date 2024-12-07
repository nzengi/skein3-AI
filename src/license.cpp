#include "license.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include "../include/base64.h"

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
    return current_license_.type != LicenseType::NON_COMMERCIAL && 
           current_license_.is_valid;
}

void LicenseManager::setLicense(const std::string& license_key) {
    if (!validateLicense(license_key)) {
        throw LicenseException("Invalid license key");
    }
}

// Diğer private fonksiyonlar... 