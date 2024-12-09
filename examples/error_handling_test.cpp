#include "skein3.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>
#include <functional>

// Custom error classes
class Skein3Error : public std::runtime_error {
public:
    explicit Skein3Error(const std::string& msg) : std::runtime_error(msg) {}
};

class ConfigurationError : public Skein3Error {
public:
    explicit ConfigurationError(const std::string& msg) : Skein3Error(msg) {}
};

class MemoryError : public Skein3Error {
public:
    explicit MemoryError(const std::string& msg) : Skein3Error(msg) {}
};

class ValidationError : public Skein3Error {
public:
    explicit ValidationError(const std::string& msg) : Skein3Error(msg) {}
};

// Test class
class ErrorHandlingTest {
public:
    static void runAllTests() {
        std::cout << "Starting error handling tests...\n\n";

        testConfigurationErrors();
        testMemoryErrors();
        testValidationErrors();
        testRecoveryMechanisms();
        testErrorLogging();
    }

private:
    static void testConfigurationErrors() {
        std::cout << "1. Configuration Errors Test\n";

        try {
            Skein3::Config config;
            config.size = static_cast<Skein3::HashSize>(999);
            std::vector<uint8_t> data{1, 2, 3};
            auto hash = Skein3::hash(data, config);
            std::cout << "Error: Invalid size accepted!\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "Expected error caught: " << e.what() << "\n";
        }

        try {
            Skein3::Config config;
            config.mode = Skein3::HashMode::TREE;
            config.tree_fan_out = 0;
            std::vector<uint8_t> data{1, 2, 3};
            auto hash = Skein3::hash(data, config);
            std::cout << "Error: Invalid thread count accepted!\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "Expected error caught: " << e.what() << "\n";
        }
    }

    static void testMemoryErrors() {
        std::cout << "\n2. Memory Errors Test\n";

        try {
            const size_t reasonable_size = 1024 * 1024 * 100; // 100MB
            std::vector<uint8_t> data(reasonable_size);
            
            Skein3::Config config;
            config.mem_protection = Skein3::MemoryProtectionMode::STANDARD;
            
            auto hash = Skein3::hash(data, config);
            std::cout << "Successfully processed " << reasonable_size << " bytes\n";
            
        } catch (const std::bad_alloc& e) {
            std::cout << "Memory allocation failed: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    static void testValidationErrors() {
        std::cout << "\n3. Validation Errors Test\n";

        try {
            std::vector<uint8_t> empty_data;
            Skein3::Config config;
            auto hash = Skein3::hash(empty_data, config);
            if (hash.empty()) {
                throw ValidationError("Empty hash generated");
            }
            std::cout << "Empty data processed, hash size: " << hash.size() << "\n";
        } catch (const ValidationError& e) {
            std::cout << "Validation error: " << e.what() << "\n";
        }

        try {
            std::vector<uint8_t> data{1, 2, 3};
            Skein3::Config config;
            auto hash = Skein3::hash(data, config);
            
            hash[0] ^= 0xFF;
            
            std::vector<uint8_t> verify_data{1, 2, 3};
            auto verify_hash = Skein3::hash(verify_data, config);
            
            if (hash != verify_hash) {
                throw ValidationError("Hash validation failed");
            }
        } catch (const ValidationError& e) {
            std::cout << "Expected validation error: " << e.what() << "\n";
        }
    }

    static void testRecoveryMechanisms() {
        std::cout << "\n4. Recovery Mechanisms Test\n";

        try {
            std::vector<uint8_t> data(1024 * 1024);
            Skein3::Config config;
            
            Skein3::createCheckpoint(data);
            
            throw std::runtime_error("Simulated error");
            
        } catch (const std::exception&) {
            try {
                auto recovered_data = Skein3::recoverFromError();
                std::cout << "Successful recovery, data size: " 
                         << recovered_data.size() << "\n";
            } catch (const std::exception& e) {
                std::cout << "Recovery error: " << e.what() << "\n";
            }
        }
    }

    static void testErrorLogging() {
        std::cout << "\n5. Error Logging Test\n";

        struct ErrorLog {
            std::string error_type;
            std::string message;
            std::chrono::system_clock::time_point timestamp;
        };

        std::vector<ErrorLog> error_logs;

        auto logError = [&error_logs](const std::string& type, 
                                    const std::string& msg) {
            error_logs.push_back({
                type,
                msg,
                std::chrono::system_clock::now()
            });
        };

        try {
            throw ConfigurationError("Test configuration error");
        } catch (const std::exception& e) {
            logError("Configuration", e.what());
        }

        try {
            throw MemoryError("Test memory error");
        } catch (const std::exception& e) {
            logError("Memory", e.what());
        }

        std::cout << "Total log count: " << error_logs.size() << "\n";
        for (const auto& log : error_logs) {
            std::cout << "Type: " << log.error_type 
                     << ", Message: " << log.message << "\n";
        }
    }
};

int main() {
    try {
        ErrorHandlingTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 