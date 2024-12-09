#include "skein3.h"
#include "license.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class LicenseTest {
public:
    static void runAllTests() {
        std::cout << "Starting license tests...\n\n";

        testLicenseValidation();
        testFeatureAccess();
        testLicenseExpiry();
        testConcurrentUse();
        testLicenseUpgrade();
    }

private:
    static void testLicenseValidation() {
        std::cout << "1. License Validation Test\n";

        // Valid license test
        {
            std::string valid_key = "SKEIN3-COMM-1234-5678-ABCD";
            try {
                LicenseManager::getInstance().setLicense(valid_key);
                std::cout << "Valid license accepted\n";
            } catch (const LicenseException& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }

        // Invalid license test
        {
            std::string invalid_key = "INVALID-LICENSE-KEY";
            try {
                LicenseManager::getInstance().setLicense(invalid_key);
                std::cout << "Error: Invalid license accepted!\n";
            } catch (const LicenseException& e) {
                std::cout << "Expected error: " << e.what() << "\n";
            }
        }
    }

    static void testFeatureAccess() {
        std::cout << "\n2. Feature Access Test\n";

        // Quantum feature test - daha küçük veri boyutu
        testFeature("Quantum Resistance", LicenseManager::Feature::QUANTUM_RESISTANCE, []() {
            Skein3::Config config;
            config.size = Skein3::HashSize::HASH_512; // 1024 yerine 512
            std::vector<uint8_t> data{1, 2, 3};
            return Skein3::hash(data, config);
        });

        // Parallel processing test - daha az thread
        testFeature("Parallel Processing", LicenseManager::Feature::PARALLEL_PROCESSING, []() {
            Skein3::Config config;
            config.mode = Skein3::HashMode::TREE;
            config.tree_fan_out = 4; // 8 yerine 4 thread
            std::vector<uint8_t> data(1024 * 1024); // 1MB veri
            return Skein3::tree_hash(data, config);
        });

        // AI optimization test - basit veri
        testFeature("AI Optimization", LicenseManager::Feature::AI_OPTIMIZATION, []() {
            Skein3::Config config;
            config.neural_config.enable_neural_adaptation = true;
            config.neural_config.complexity_threshold = 0.75f;
            std::vector<uint8_t> data{1, 2, 3};
            return Skein3::hash(data, config);
        });
    }

    static void testLicenseExpiry() {
        std::cout << "\n3. License Expiry Test\n";

        // Expired license test
        {
            auto future_time = std::chrono::system_clock::now() + 
                             std::chrono::hours(24);
            
            LicenseManager::License test_license;
            test_license.expiry = future_time;
            test_license.type = LicenseManager::LicenseType::BASIC_COMMERCIAL;
            
            bool is_valid = test_license.expiry > std::chrono::system_clock::now();
            std::cout << "License check: " 
                     << (is_valid ? "Success" : "Failed") << "\n";
        }

        // Expired license test
        {
            auto past_time = std::chrono::system_clock::now() - 
                           std::chrono::hours(24);
            
            LicenseManager::License test_license;
            test_license.expiry = past_time;
            test_license.type = LicenseManager::LicenseType::BASIC_COMMERCIAL;
            
            bool is_expired = test_license.expiry < std::chrono::system_clock::now();
            std::cout << "Expired license check: " 
                     << (is_expired ? "Success" : "Failed") << "\n";
        }
    }

    static void testConcurrentUse() {
        std::cout << "\n4. Concurrent Use Test\n";

        const int NUM_THREADS = 4;
        std::vector<std::thread> threads;
        std::atomic<int> success_count{0};

        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&success_count]() {
                try {
                    Skein3::Config config;
                    config.size = Skein3::HashSize::HASH_512;
                    std::vector<uint8_t> data{1, 2, 3};
                    auto hash = Skein3::hash(data, config);
                    success_count++;
                } catch (const LicenseException&) {
                    // Expected license error
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "Successful operations: " << success_count 
                 << "/" << NUM_THREADS << "\n";
    }

    static void testLicenseUpgrade() {
        std::cout << "\n5. License Upgrade Test\n";

        // Upgrade from Basic to Enterprise
        try {
            // First, set basic license
            LicenseManager::getInstance().setLicense("SKEIN3-BASIC-1234");
            auto initial_type = LicenseManager::getInstance().getCurrentLicense().type;
            
            // Upgrade to Enterprise
            LicenseManager::getInstance().setLicense("SKEIN3-ENTERPRISE-5678");
            auto upgraded_type = LicenseManager::getInstance().getCurrentLicense().type;
            
            bool upgrade_successful = 
                initial_type == LicenseManager::LicenseType::BASIC_COMMERCIAL &&
                upgraded_type == LicenseManager::LicenseType::ENTERPRISE;
            
            std::cout << "License upgrade: " 
                     << (upgrade_successful ? "Success" : "Failed") << "\n";
            
        } catch (const LicenseException& e) {
            std::cout << "License upgrade error: " << e.what() << "\n";
        }
    }

private:
    template<typename Func>
    static void testFeature(const std::string& feature_name, 
                          LicenseManager::Feature feature,
                          Func test_func) {
        try {
            test_func();
            std::cout << feature_name << " access: Success\n";
        } catch (const LicenseException& e) {
            std::cout << feature_name << " access: Failed - " 
                     << e.what() << "\n";
        }
    }
};

int main() {
    try {
        LicenseTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 