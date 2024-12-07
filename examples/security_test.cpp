#include "skein3.h"
#include "security_monitor.h"
#include "adaptive_security.h"
#include <iostream>  // std::cout için
#include <string>   // std::string için

void testSecurity() {
    std::string test_data = "Güvenlik testi verisi";
    std::vector<uint8_t> data(test_data.begin(), test_data.end());

    // Güvenlik analizi
    Skein3::Config config;
    auto hash = Skein3::hash(data, config);
    
    auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);
    auto threat_level = AdaptiveSecurity::analyzeThreatLevel(data, metrics);

    std::cout << "Entropi seviyesi: " << metrics.entropy_level << std::endl;
    std::cout << "Pattern karmaşıklığı: " << metrics.pattern_complexity << std::endl;
    std::cout << "Saldırı olasılığı: " << metrics.attack_probability << std::endl;
    std::cout << "Tehdit seviyesi: " << static_cast<int>(threat_level) << std::endl;
} 

int main() {
    testSecurity();
    return 0;
} 