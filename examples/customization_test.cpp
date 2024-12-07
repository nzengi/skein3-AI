#include "skein3.h"
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

class CustomHashMode : public Skein3 {
public:
    enum class CustomFeature {
        BLOCKCHAIN_SPECIFIC,
        IOT_OPTIMIZED,
        REAL_TIME_PROCESSING
    };

    struct CustomConfig : public Config {
        CustomFeature custom_feature = CustomFeature::BLOCKCHAIN_SPECIFIC;
        bool enable_custom_optimization = false;
        std::function<void(const std::vector<uint8_t>&)> callback;
    };

    static std::vector<uint8_t> customHash(
        const std::vector<uint8_t>& data,
        const CustomConfig& config
    ) {
        if (config.callback) {
            config.callback(data);
        }

        switch (config.custom_feature) {
            case CustomFeature::BLOCKCHAIN_SPECIFIC:
                return blockchainOptimizedHash(data, config);
            case CustomFeature::IOT_OPTIMIZED:
                return iotOptimizedHash(data, config);
            case CustomFeature::REAL_TIME_PROCESSING:
                return realTimeHash(data, config);
            default:
                return Skein3::hash(data, config);
        }
    }

private:
    static std::vector<uint8_t> blockchainOptimizedHash(
        const std::vector<uint8_t>& data,
        const CustomConfig& config
    ) {
        Config modified_config = config;
        modified_config.merkle_tree = true;
        modified_config.zero_knowledge = true;
        
        auto initial_hash = Skein3::hash(data, modified_config);
        return Skein3::hash(initial_hash, modified_config);
    }

    static std::vector<uint8_t> iotOptimizedHash(
        const std::vector<uint8_t>& data,
        const CustomConfig& config
    ) {
        Config modified_config = config;
        modified_config.size = HashSize::HASH_256;
        modified_config.mem_protection = MemoryProtectionMode::STANDARD;
        
        return Skein3::hash(data, modified_config);
    }

    static std::vector<uint8_t> realTimeHash(
        const std::vector<uint8_t>& data,
        const CustomConfig& config
    ) {
        Config modified_config = config;
        modified_config.mode = HashMode::STREAMING;
        modified_config.tree_fan_out = 2;
        
        return Skein3::hash(data, modified_config);
    }
};

class CustomizationTest {
public:
    static void runAllTests() {
        std::cout << "Starting customization tests...\n\n";

        testCustomHashModes();
        testCallbacks();
        testExtension();
        testOptimizations();
        testIntegration();
    }

private:
    static void testCustomHashModes() {
        std::cout << "1. Custom Hash Modes Test\n";

        std::vector<uint8_t> test_data{1, 2, 3, 4, 5};
        CustomHashMode::CustomConfig config;

        {
            config.custom_feature = CustomHashMode::CustomFeature::BLOCKCHAIN_SPECIFIC;
            auto hash = CustomHashMode::customHash(test_data, config);
            std::cout << "Blockchain mode hash size: " << hash.size() << "\n";
        }

        {
            config.custom_feature = CustomHashMode::CustomFeature::IOT_OPTIMIZED;
            auto hash = CustomHashMode::customHash(test_data, config);
            std::cout << "IoT mode hash size: " << hash.size() << "\n";
        }

        {
            config.custom_feature = CustomHashMode::CustomFeature::REAL_TIME_PROCESSING;
            auto hash = CustomHashMode::customHash(test_data, config);
            std::cout << "Real-time mode hash size: " << hash.size() << "\n";
        }
    }

    static void testCallbacks() {
        std::cout << "\n2. Callback Functions Test\n";

        std::vector<uint8_t> test_data{1, 2, 3, 4, 5};
        CustomHashMode::CustomConfig config;

        bool callback_called = false;
        config.callback = [&callback_called](const std::vector<uint8_t>& data) {
            callback_called = true;
            std::cout << "Callback called, data size: " << data.size() << "\n";
        };

        auto hash = CustomHashMode::customHash(test_data, config);
        std::cout << "Callback status: " << (callback_called ? "Success" : "Failed") << "\n";
    }

    static void testExtension() {
        std::cout << "\n3. Extension Test\n";

        struct CustomData {
            std::vector<uint8_t> data;
            std::string metadata;
        };

        auto customHashFunction = [](const CustomData& custom_data) {
            CustomHashMode::CustomConfig config;
            config.custom_feature = CustomHashMode::CustomFeature::BLOCKCHAIN_SPECIFIC;
            
            std::vector<uint8_t> combined_data = custom_data.data;
            combined_data.insert(combined_data.end(), 
                               custom_data.metadata.begin(), 
                               custom_data.metadata.end());
            
            return CustomHashMode::customHash(combined_data, config);
        };

        CustomData test_data{{1, 2, 3}, "test_metadata"};
        auto hash = customHashFunction(test_data);
        std::cout << "Extended hash size: " << hash.size() << "\n";
    }

    static void testOptimizations() {
        std::cout << "\n4. Optimization Test\n";

        std::vector<uint8_t> test_data(1024 * 1024);
        CustomHashMode::CustomConfig config;

        {
            auto start = std::chrono::high_resolution_clock::now();
            auto hash = Skein3::hash(test_data, config);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start
            ).count();
            std::cout << "Normal mode duration: " << duration << "ms\n";
        }

        {
            config.enable_custom_optimization = true;
            auto start = std::chrono::high_resolution_clock::now();
            auto hash = CustomHashMode::customHash(test_data, config);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start
            ).count();
            std::cout << "Optimized mode duration: " << duration << "ms\n";
        }
    }

    static void testIntegration() {
        std::cout << "\n5. Integration Test\n";

        class CustomStreamProcessor {
        public:
            void processChunk(const std::vector<uint8_t>& chunk) {
                CustomHashMode::CustomConfig config;
                config.custom_feature = CustomHashMode::CustomFeature::REAL_TIME_PROCESSING;
                
                auto hash = CustomHashMode::customHash(chunk, config);
                processed_chunks_++;
                
                std::cout << "Chunk " << processed_chunks_ 
                         << " processed, hash size: " << hash.size() << "\n";
            }
        private:
            size_t processed_chunks_ = 0;
        };

        CustomStreamProcessor processor;
        std::vector<uint8_t> chunk(1024);
        
        for (int i = 0; i < 5; ++i) {
            processor.processChunk(chunk);
        }
    }
};

int main() {
    try {
        CustomizationTest::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
} 