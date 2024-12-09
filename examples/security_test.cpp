#include "skein3.h"
#include "security_monitor.h"
#include "adaptive_security.h"
#include <iostream> 
#include <string> 

void testSecurity() {
    try {
        std::string test_data = "Security test data";
        std::vector<uint8_t> data(test_data.begin(), test_data.end());

        Skein3::Config config;
        
        try {
            auto hash = Skein3::hash(data, config);
            auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);
            auto threat_level = AdaptiveSecurity::analyzeThreatLevel(data, metrics);

            std::cout << "Security Analysis Results:\n";
            std::cout << "Entropy level: " << metrics.entropy_level << "\n";
            std::cout << "Pattern complexity: " << metrics.pattern_complexity << "\n";
            std::cout << "Attack probability: " << metrics.attack_probability << "\n";
            std::cout << "Threat level: " << static_cast<int>(threat_level) << "\n";
            
            if (!Skein3::verifyMemoryIntegrity(hash.data(), hash.size(), config)) {
                throw std::runtime_error("Memory integrity check failed");
            }
            
        } catch (const LicenseException& e) {
            std::cout << "License warning: " << e.what() << "\n";
            std::cout << "Using standard security features\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Security test error: " << e.what() << std::endl;
    }
}

int main() {
    testSecurity();
    return 0;
} 