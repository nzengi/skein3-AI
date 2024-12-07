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
        try {
            // 1. Simple hash calculation
            std::string message = "Test message";
            std::vector<uint8_t> data(message.begin(), message.end());
            
            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
            
            if (hash.empty()) return false;

            // 2. Different hash sizes
            config.size = Skein3::HashSize::HASH_1024;
            auto large_hash = Skein3::hash(data, config);
            
            if (large_hash.size() != 1024/8) return false;

            // 3. Streaming API
            Skein3::StreamingHasher hasher(config);
            hasher.update(data);
            auto stream_hash = hasher.finalize();
            
            if (stream_hash.empty()) return false;

            return true;
        } catch (...) {
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

            // Dosyayı temizle
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

            // Stream'den parça parça oku ve işle
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
        try {
            // 1. Empty data test
            std::vector<uint8_t> empty_data;
            Skein3::Config config;
            auto hash1 = Skein3::hash(empty_data, config);

            // 2. Invalid configuration test
            config.tree_fan_out = 0;  // Invalid value
            bool caught_exception = false;
            try {
                auto hash2 = Skein3::hash(empty_data, config);
            } catch (const std::invalid_argument&) {
                caught_exception = true;
            }

            // 3. License check
            config.size = Skein3::HashSize::HASH_1024;
            bool license_check = false;
            try {
                auto hash3 = Skein3::hash(empty_data, config);
            } catch (const LicenseException&) {
                license_check = true;
            }

            return !hash1.empty() && caught_exception && license_check;
        } catch (...) {
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