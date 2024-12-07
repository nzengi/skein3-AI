#include "skein3.h"
#include "security_monitor.h"
#include "adaptive_security.h"
#include <iostream> 
#include <string> 

void testSecurity() {
    std::string test_data = "Security test data";
    std::vector<uint8_t> data(test_data.begin(), test_data.end());

    // Security analysis
    Skein3::Config config;
    auto hash = Skein3::hash(data, config);
    
    auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);
    auto threat_level = AdaptiveSecurity::analyzeThreatLevel(data, metrics);

    std::cout << "Entropy level: " << metrics.entropy_level << std::endl;
    std::cout << "Pattern complexity: " << metrics.pattern_complexity << std::endl;
    std::cout << "Attack probability: " << metrics.attack_probability << std::endl;
    std::cout << "Threat level: " << static_cast<int>(threat_level) << std::endl;
} 

int main() {
    testSecurity();
    return 0;
} 