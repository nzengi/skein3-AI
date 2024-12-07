#include "skein3.h"
#include "license.h"
#include <iostream>

int main() {
    try {
        // Set commercial license
        LicenseManager::getInstance().setLicense("YOUR-LICENSE-KEY-HERE");

        // Now you can use commercial features
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_1024;  // Quantum-resistant mode
        config.mode = Skein3::HashMode::TREE;
        config.tree_fan_out = 8;  // Use 8 threads

        std::vector<uint8_t> message = {1, 2, 3, 4, 5};
        auto hash = Skein3::hash(message, config);

    } catch (const LicenseException& e) {
        std::cerr << "License error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 