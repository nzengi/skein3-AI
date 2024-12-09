#include "skein3.h"
#include "security_monitor.h"
#include "adaptive_security.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

void printHash(const std::string& label, const std::vector<uint8_t>& hash) {
    std::cout << label << ": ";
    for (const uint8_t byte : hash) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

void printSecurityMetrics(const SecurityMonitor::SecurityMetrics& metrics) {
    std::cout << "\nSecurity Metrics:" << std::endl;
    std::cout << "Entropy Level: " << metrics.entropy_level << std::endl;
    std::cout << "Pattern Complexity: " << metrics.pattern_complexity << std::endl;
    std::cout << "Attack Probability: " << metrics.attack_probability << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        // Lisans anahtarını çevresel değişkenden al
        const char* license_key = std::getenv("SKEIN3_LICENSE");
        if (!license_key) {
            std::cerr << "License key not found. Please set SKEIN3_LICENSE environment variable.\n";
            std::cerr << "Contact: your@email.com\n";
            return 1;
        }

        // Lisansı ayarla
        std::cout << "Setting license key: " << license_key << std::endl;  // Debug mesajı
        LicenseManager::getInstance().setLicense(license_key);

        // Lisans kontrolü
        if (!LicenseManager::getInstance().isLicenseValid()) {
            std::cerr << "No valid license found. Please obtain a license key.\n";
            std::cerr << "Contact: your@email.com\n";
            return 1;
        }

        // Example message
        std::string message = "This is a C++ implementation of the Skein3 hash function.";
        std::vector<uint8_t> message_bytes(message.begin(), message.end());

        // Basic hash with standard configuration
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_512;
        config.mode = Skein3::HashMode::STANDARD;
        
        auto hash = Skein3::hash(message_bytes, config);
        printHash("Standard Hash", hash);

        // Security monitoring
        auto metrics = SecurityMonitor::analyzeHashOperation(message_bytes, hash);
        printSecurityMetrics(metrics);

        // Adaptive security based on threat level
        auto threat_level = AdaptiveSecurity::analyzeThreatLevel(message_bytes, metrics);
        AdaptiveSecurity::adjustSecurityParameters(config, threat_level);

        // Neural adaptation
        config.neural_config.enable_neural_adaptation = true;
        config.neural_config.complexity_threshold = 0.85f;
        auto adapted_hash = Skein3::hash(message_bytes, config);
        printHash("Neural Adapted Hash", adapted_hash);

        // Parallel tree mode with memory protection
        config.mode = Skein3::HashMode::TREE;
        config.tree_fan_out = 4;
        config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;
        auto tree_hash = Skein3::tree_hash(message_bytes, config);
        printHash("Protected Tree Hash", tree_hash);

        // Batch processing for AI workloads
        std::vector<std::vector<uint8_t>> batch_messages;
        for (int i = 0; i < 5; i++) {
            batch_messages.push_back(message_bytes);
        }
        
        config.batch_processing = true;
        config.opt_mode = Skein3::OptimizationMode::AI_TRAINING;
        auto batch_hashes = Skein3::batch_hash(batch_messages, config);
        
        std::cout << "\nBatch Processing Results:" << std::endl;
        for (size_t i = 0; i < batch_hashes.size(); i++) {
            printHash("Batch " + std::to_string(i), batch_hashes[i]);
        }

        // Blockchain features
        std::vector<std::vector<uint8_t>> transactions = batch_messages;
        config.opt_mode = Skein3::OptimizationMode::BLOCKCHAIN;
        config.merkle_tree = true;
        
        auto merkle_root = Skein3::merkle_root(transactions, config);
        printHash("Merkle Root", merkle_root);

        // Performance metrics
        std::cout << "\nPerformance Test:" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        
        const size_t num_iterations = 10000;
        for (size_t i = 0; i < num_iterations; i++) {
            Skein3::hash(message_bytes, config);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        ).count();
        
        double hashes_per_second = (num_iterations * 1000000.0) / duration;
        std::cout << "Hashes per second: " << std::fixed << std::setprecision(2) 
                 << hashes_per_second << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
