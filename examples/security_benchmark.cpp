#include "skein3.h"
#include "security_monitor.h"
#include "adaptive_security.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <fstream>
#include <unordered_set>
#include <utility>
#include <algorithm>

class SecurityBenchmark {
public:
    struct SecurityTestResult {
        float entropy_level;
        float pattern_complexity;
        float attack_probability;
        AdaptiveSecurity::ThreatLevel threat_level;
        bool quantum_resistant;
        double collision_resistance;
    };

    static void runSecurityTests() {
        std::cout << "Security tests starting...\n\n";

        // Different test scenarios
        testStandardAttacks();
        testQuantumResistance();
        testCollisionResistance();
        testEntropyLevels();
        testAdaptiveSecurity();
    }

private:
    static void testStandardAttacks() {
        std::cout << "1. Standard Attack Resistance Test\n";
        
        // Test data
        std::vector<std::vector<uint8_t>> test_cases = {
            generateSequentialData(1024),    // Sequential data
            generateRepeatingData(1024),     // Repeating data
            generateRandomData(1024)         // Random data
        };

        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_512;

        for (const auto& data : test_cases) {
            auto hash = Skein3::hash(data, config);
            auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);

            std::cout << "Entropy Level: " << metrics.entropy_level << "\n";
            std::cout << "Attack Probability: " << metrics.attack_probability << "\n\n";
        }
    }

    static void testQuantumResistance() {
        std::cout << "2. Quantum Resistance Test\n";
        
        Skein3::Config config;
        config.size = Skein3::HashSize::HASH_1024;
        config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;

        auto data = generateRandomData(1024 * 1024); // 1MB veri
        auto hash = Skein3::hash(data, config);

        // Analyze quantum properties
        analyzeQuantumProperties(hash);
    }

    static void testCollisionResistance() {
        std::cout << "3. Collision Resistance Test\n";
        
        const size_t NUM_TESTS = 1000000;
        std::unordered_set<std::string> hashes;
        
        Skein3::Config config;
        for (size_t i = 0; i < NUM_TESTS; ++i) {
            auto data = generateRandomData(64);
            auto hash = Skein3::hash(data, config);
            
            std::string hash_str(hash.begin(), hash.end());
            if (hashes.find(hash_str) != hashes.end()) {
                std::cout << "Collision found! Test #" << i << "\n";
                return;
            }
            hashes.insert(hash_str);
        }
        
        std::cout << "Collision not found (" << NUM_TESTS << " tests)\n";
    }

    static void testEntropyLevels() {
        std::cout << "4. Entropy Level Test\n";
        
        // Create data with different entropy levels
        std::vector<std::pair<std::string, std::vector<uint8_t>>> test_data = {
            {"Low Entropy", generateLowEntropyData(1024)},
            {"Medium Entropy", generateMediumEntropyData(1024)},
            {"High Entropy", generateHighEntropyData(1024)}
        };

        Skein3::Config config;
        for (const auto& [desc, data] : test_data) {
            auto hash = Skein3::hash(data, config);
            auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);
            
            std::cout << desc << ":\n";
            std::cout << "Entropy Level: " << metrics.entropy_level << "\n";
            std::cout << "Pattern Complexity: " << metrics.pattern_complexity << "\n\n";
        }
    }

    static void testAdaptiveSecurity() {
        std::cout << "5. Adaptive Security Test\n";
        
        // Test by increasing threat level
        std::vector<uint8_t> data = generateRandomData(1024);
        Skein3::Config config;

        for (int i = 0; i < 4; ++i) {
            auto hash = Skein3::hash(data, config);
            auto metrics = SecurityMonitor::analyzeHashOperation(data, hash);
            auto threat_level = AdaptiveSecurity::analyzeThreatLevel(data, metrics);
            
            std::cout << "Iteration " << i << ":\n";
            std::cout << "Threat Level: " << static_cast<int>(threat_level) << "\n";
            
            // Adjust security parameters
            AdaptiveSecurity::adjustSecurityParameters(config, threat_level);
        }
    }

    // Helper functions
    static std::vector<uint8_t> generateRandomData(size_t size) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        std::vector<uint8_t> data(size);
        std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
        return data;
    }

    static std::vector<uint8_t> generateSequentialData(size_t size) {
        std::vector<uint8_t> data(size);
        std::iota(data.begin(), data.end(), 0);
        return data;
    }

    static std::vector<uint8_t> generateRepeatingData(size_t size) {
        std::vector<uint8_t> data(size);
        for (size_t i = 0; i < size; ++i) {
            data[i] = i % 256;
        }
        return data;
    }

    static void analyzeQuantumProperties(const std::vector<uint8_t>& hash) {
        // Analyze quantum properties
        double quantum_score = calculateQuantumScore(hash);
        std::cout << "Quantum Resistance Score: " << quantum_score << "\n";
    }

    static double calculateQuantumScore(const std::vector<uint8_t>& hash) {
        // Simple quantum score calculation
        double entropy = 0.0;
        std::array<int, 256> freq{};
        
        for (uint8_t byte : hash) {
            freq[byte]++;
        }
        
        for (int count : freq) {
            if (count > 0) {
                double p = static_cast<double>(count) / hash.size();
                entropy -= p * std::log2(p);
            }
        }
        
        return entropy / 8.0; // Normalized score
    }

    static std::vector<uint8_t> generateLowEntropyData(size_t size) {
        std::vector<uint8_t> data(size, 0x00);
        return data;
    }

    static std::vector<uint8_t> generateMediumEntropyData(size_t size) {
        std::vector<uint8_t> data(size);
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<uint8_t>(i % 16);
        }
        return data;
    }

    static std::vector<uint8_t> generateHighEntropyData(size_t size) {
        std::vector<uint8_t> data(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        std::generate(data.begin(), data.end(), 
                     [&]() { return static_cast<uint8_t>(dis(gen)); });
        return data;
    }
};

int main() {
    try {
        SecurityBenchmark::runSecurityTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 