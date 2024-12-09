#include "skein3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>

class IntegrationTest {
public:
    struct TestCase {
        std::string name;
        std::function<bool()> test_func;
        std::string description;
    };

    static void runAllTests() {
        std::cout << "Starting integration tests...\n\n";

        std::vector<TestCase> test_cases = {
            {"API_Basic", testBasicAPI, "Basic API usage"},
            {"File_Integration", testFileIntegration, "File system integration"},
            {"Stream_Processing", testStreamProcessing, "Stream processing integration"},
            {"Error_Handling", testErrorHandling, "Error handling"},
            {"Config_Management", testConfigManagement, "Configuration management"}
        };

        int passed = 0;
        int failed = 0;

        for (const auto& test : test_cases) {
            std::cout << "\nTest: " << test.name << "\n";
            std::cout << "Description: " << test.description << "\n";
            
            try {
                bool result = test.test_func();
                if (result) {
                    std::cout << "Result: ✅ Success\n";
                    passed++;
                } else {
                    std::cout << "Result: ❌ Failed\n";
                    failed++;
                }
            } catch (const std::exception& e) {
                std::cout << "Result: ❌ Error: " << e.what() << "\n";
                failed++;
            }
        }

        std::cout << "\nTotal Result:\n";
        std::cout << "Success: " << passed << "\n";
        std::cout << "Failed: " << failed << "\n";
    }

private:
    // Basic API usage test
    static bool testBasicAPI() {
        std::cout << "\nAPI Basic Test\n";
        try {
            // 1. Simple hash calculation
            std::cout << "Testing basic hash calculation... ";
            std::string message = "Test message";
            std::vector<uint8_t> data(message.begin(), message.end());
            
            Skein3::Config config;
            config.size = Skein3::HashSize::HASH_256; // Explicitly set size
            auto hash = Skein3::hash(data, config);
            
            if (hash.empty()) {
                std::cout << "Failed: Empty hash returned\n";
                return false;
            }
            std::cout << "Success (hash size: " << hash.size() << " bytes)\n";

            // 2. Different hash sizes
            std::cout << "Testing different hash sizes... ";
            config.size = Skein3::HashSize::HASH_512;
            auto large_hash = Skein3::hash(data, config);
            
            if (large_hash.size() != 512/8) {
                std::cout << "Failed: Incorrect hash size (got " 
                         << large_hash.size() << ", expected " << 512/8 << ")\n";
                return false;
            }
            std::cout << "Success (hash size: " << large_hash.size() << " bytes)\n";

            // 3. Streaming API
            std::cout << "Testing streaming API... ";
            Skein3::StreamingHasher hasher(config);
            hasher.update(data);
            auto stream_hash = hasher.finalize();
            
            if (stream_hash.empty()) {
                std::cout << "Failed: Empty stream hash\n";
                return false;
            }
            std::cout << "Success (hash size: " << stream_hash.size() << " bytes)\n";

            return true;
        } catch (const std::exception& e) {
            std::cout << "Failed: " << e.what() << "\n";
            return false;
        }
    }

    // File system integration test
    static bool testFileIntegration() {
        try {
            // Create test file
            std::string filename = "test_file.txt";
            std::string content = "Test file content";
            
            {
                std::ofstream file(filename);
                file << content;
            }

            // Calculate hash from file
            std::vector<uint8_t> file_data;
            {
                std::ifstream file(filename, std::ios::binary);
                file_data = std::vector<uint8_t>(
                    std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>()
                );
            }

            Skein3::Config config;
            auto hash = Skein3::hash(file_data, config);

            std::remove(filename.c_str());

            return !hash.empty();
        } catch (...) {
            return false;
        }
    }

    // Stream processing test
    static bool testStreamProcessing() {
        try {
            // Simulate large data stream
            std::stringstream stream;
            for (int i = 0; i < 1000; ++i) {
                stream << "Block " << i << " ";
            }

            Skein3::Config config;
            Skein3::StreamingHasher hasher(config);

            char buffer[256];
            while (stream.read(buffer, sizeof(buffer))) {
                std::vector<uint8_t> chunk(buffer, buffer + stream.gcount());
                hasher.update(chunk);
            }

            auto final_hash = hasher.finalize();
            return !final_hash.empty();
        } catch (...) {
            return false;
        }
    }

    // Error handling test
    static bool testErrorHandling() {
        std::cout << "\n4. Error Handling Test\n";

        try {
            // 1. Empty data test
            std::cout << "Testing empty data handling... ";
            std::vector<uint8_t> empty_data;
            Skein3::Config config;
            auto hash1 = Skein3::hash(empty_data, config);
            if (hash1.empty()) {
                std::cout << "Failed: Empty hash returned\n";
                return false;
            }
            std::cout << "Success\n";

            // 2. Invalid configuration test
            std::cout << "Testing invalid configuration... ";
            config.mode = Skein3::HashMode::TREE;
            config.tree_fan_out = 0;  // Invalid value
            bool caught_exception = false;
            try {
                auto hash2 = Skein3::hash(empty_data, config);
                std::cout << "Failed: Should throw invalid_argument\n";
                return false;
            } catch (const std::invalid_argument&) {
                caught_exception = true;
                std::cout << "Success\n";
            }

            // 3. License check - Config'i sıfırla
            std::cout << "Testing license validation... ";
            config = Skein3::Config(); // Yeni temiz config
            config.size = Skein3::HashSize::HASH_1024;
            bool license_check = false;
            try {
                auto hash3 = Skein3::hash(empty_data, config);
                std::cout << "Failed: Should throw LicenseException\n";
                return false;
            } catch (const LicenseException&) {
                license_check = true;
                std::cout << "Success\n";
            }

            // 4. Memory protection - Config'i sıfırla
            std::cout << "Testing memory protection... ";
            config = Skein3::Config(); // Yeni temiz config
            config.mem_protection = Skein3::MemoryProtectionMode::QUANTUM_RESISTANT;
            try {
                std::vector<uint8_t> large_data(1024 * 1024); // 1MB
                auto hash4 = Skein3::hash(large_data, config);
                std::cout << "Success\n";
            } catch (const std::exception& e) {
                std::cout << "Failed: " << e.what() << "\n";
                return false;
            }

            return true;
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
            return false;
        }
    }

    // Configuration management test
    static bool testConfigManagement() {
        try {
            Skein3::Config config;

            // 1. Basic configuration
            config.size = Skein3::HashSize::HASH_512;
            config.mode = Skein3::HashMode::STANDARD;
            
            std::vector<uint8_t> data{1, 2, 3};
            auto hash1 = Skein3::hash(data, config);

            // 2. AI optimization
            Skein3::optimize_for_ai(config);
            auto hash2 = Skein3::hash(data, config);

            // 3. Blockchain optimization
            Skein3::optimize_for_blockchain(config);
            auto hash3 = Skein3::hash(data, config);

            // 4. Neural adaptation
            config.neural_config.enable_neural_adaptation = true;
            config.neural_config.complexity_threshold = 0.85f;
            auto hash4 = Skein3::hash(data, config);

            return !hash1.empty() && !hash2.empty() && 
                   !hash3.empty() && !hash4.empty();
        } catch (...) {
            return false;
        }
    }
};

int main() {
    try {
        IntegrationTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 