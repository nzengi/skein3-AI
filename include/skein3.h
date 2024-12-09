#ifndef SKEIN3_H
#define SKEIN3_H

// Standard library includes
#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <cstring>

// Project includes
#include "threefish3.h"
#include "license.h"
#include "memory_protection.h"

/**
 * @brief Advanced cryptographic hash function with enhanced security features
 * 
 * Skein3 is an improved version of the Skein hash function, offering:
 * - Quantum resistance
 * - Variable output sizes
 * - Parallel processing capabilities
 * - Enhanced security features
 */
class Skein3 {
public:
    // Enums first
    enum class HashSize {
        HASH_256 = 256,
        HASH_512 = 512,
        HASH_1024 = 1024
    };

    enum class HashMode {
        STANDARD,
        TREE,
        STREAMING
    };

    enum class OptimizationMode {
        AI_TRAINING,
        BLOCKCHAIN,
        STANDARD
    };

    enum class MemoryProtectionMode {
        STANDARD,
        QUANTUM_RESISTANT,
        HARDWARE_BACKED
    };

    // Then structs
    struct NeuralConfig {
        bool enable_neural_adaptation;
        float complexity_threshold;
        size_t adaptation_rounds;
        std::vector<float> weights;

        NeuralConfig() 
            : enable_neural_adaptation(false)
            , complexity_threshold(0.75f)
            , adaptation_rounds(1000)
        {}
    };

    struct Config {
        HashSize size;
        HashMode mode;
        OptimizationMode opt_mode;
        size_t tree_leaf_size;
        size_t tree_fan_out;
        bool personalization;
        std::vector<uint8_t> person_string;
        
        bool batch_processing;
        size_t batch_size;
        bool gpu_acceleration;
        
        bool merkle_tree;
        bool zero_knowledge;
        size_t nonce_space;
        
        MemoryProtectionMode mem_protection;
        bool secure_memory_wipe;
        size_t memory_protection_rounds;
        
        NeuralConfig neural_config;

        Config()
            : size(HashSize::HASH_512)
            , mode(HashMode::STANDARD)
            , opt_mode(OptimizationMode::STANDARD)
            , tree_leaf_size(1024)
            , tree_fan_out(8)
            , personalization(false)
            , batch_processing(false)
            , batch_size(1024)
            , gpu_acceleration(false)
            , merkle_tree(false)
            , zero_knowledge(false)
            , nonce_space(32)
            , mem_protection(MemoryProtectionMode::STANDARD)
            , secure_memory_wipe(true)
            , memory_protection_rounds(3)
        {}
    };

    /**
     * @brief Compute hash of input message
     * @param message Input data to be hashed
     * @param config Hash configuration options
     * @return Hash value as byte vector
     */
    static std::vector<uint8_t> hash(
        const std::vector<uint8_t>& message,
        const Config& config = Config()
    );
    
    /**
     * @brief Compute MAC (Message Authentication Code)
     * @param message Input message
     * @param key Secret key
     * @param config MAC configuration
     * @return MAC value as byte vector
     */
    static std::vector<uint8_t> mac(const std::vector<uint8_t>& message,
                                  const std::vector<uint8_t>& key,
                                  const Config& config = Config());
    
    /**
     * @brief Parallel tree-based hashing
     * @param message Input message
     * @param config Tree configuration
     * @return Hash value as byte vector
     */
    static std::vector<uint8_t> tree_hash(const std::vector<uint8_t>& message,
                                        const Config& config = Config());
    
    /**
     * @brief Streaming hash processor for continuous data
     */
    class StreamingHasher {
    public:
        /**
         * @brief Initialize streaming hasher
         * @param config Hash configuration
         */
        StreamingHasher(const Config& config = Config());

        /**
         * @brief Process new chunk of data
         * @param data Input data chunk
         */
        void update(const std::vector<uint8_t>& data);

        /**
         * @brief Complete hash computation
         * @return Final hash value
         */
        std::vector<uint8_t> finalize();

    private:
        Config config_;
        std::vector<uint8_t> buffer_;
        std::array<uint64_t, Threefish3::NUM_WORDS> state_;
        size_t total_bytes_;
    };

    // New methods
    static std::vector<std::vector<uint8_t>> batch_hash(
        const std::vector<std::vector<uint8_t>>& messages,
        const Config& config = Config()
    );

    static std::vector<uint8_t> merkle_root(
        const std::vector<std::vector<uint8_t>>& transactions,
        const Config& config = Config()
    );

    static bool verify_zero_knowledge(
        const std::vector<uint8_t>& proof,
        const std::vector<uint8_t>& public_input,
        const Config& config = Config()
    );

    // GPU-accelerated hash computation
    #ifdef CUDA_ENABLED
    static std::vector<uint8_t> gpu_hash(
        const std::vector<uint8_t>& message,
        const Config& config = Config()
    );
    #endif

    // Add secure memory operations
    template<typename T>
    static std::unique_ptr<T[]> allocateSecureMemory(
        size_t size,
        const Config& config
    ) {
        QuantumResistantMemory::ProtectionConfig prot_config;
        prot_config.level = static_cast<QuantumResistantMemory::ProtectionLevel>(
            config.mem_protection
        );
        prot_config.rounds = config.memory_protection_rounds;
        prot_config.secure_wipe = config.secure_memory_wipe;
        
        return QuantumResistantMemory::allocateSecure<T>(size, prot_config);
    }

    // Add memory verification
    template<typename T>
    static bool verifyMemoryIntegrity(
        const T* memory,
        size_t size,
        const Config& config
    ) {
        QuantumResistantMemory::ProtectionConfig prot_config;
        prot_config.level = static_cast<QuantumResistantMemory::ProtectionLevel>(
            config.mem_protection
        );
        return QuantumResistantMemory::verifyIntegrity(memory, size, prot_config);
    }

    static std::vector<uint8_t> adaptHash(
        const std::vector<uint8_t>& message,
        const Config& config
    );
    
    static void optimize_for_ai(Config& config);
    static void optimize_for_blockchain(Config& config);

    // Neural network functions
    static void initializeNeuralAdapter(const Config& config);
    static void saveNeuralWeights(const std::string& filename);
    static void loadNeuralWeights(const std::string& filename);

    static void createCheckpoint(const std::vector<uint8_t>& data) {
        checkpoint_data_ = data;
        has_checkpoint_ = true;
    }

    static std::vector<uint8_t> recoverFromError() {
        if (!has_checkpoint_) {
            throw std::runtime_error("No checkpoint available");
        }
        return checkpoint_data_;
    }

    static bool verifyHash(
        const std::vector<uint8_t>& message,
        const std::vector<uint8_t>& hash,
        const Config& config = Config()
    );

    static void secureCleanup();

private:
    static void checkLicense(const Config& config) {
        auto& license_manager = LicenseManager::getInstance();
        
        // Quantum mode requires commercial license
        if (config.size == HashSize::HASH_1024 && 
            !license_manager.isCommercialUse()) {
            throw LicenseException(
                "Quantum-resistant mode requires a commercial license"
            );
        }

        // Tree mode with more than 2 threads requires commercial license
        if (config.mode == HashMode::TREE && 
            config.tree_fan_out > 2 && 
            !license_manager.isCommercialUse()) {
            throw LicenseException(
                "Parallel processing with more than 2 threads requires a commercial license"
            );
        }
    }
    
    /**
     * @brief Unique Block Iteration function
     * Core compression function using Threefish
     */
    static void UBI(std::array<uint64_t, Threefish3::NUM_WORDS>& G,
                   const std::array<uint64_t, Threefish3::NUM_WORDS>& block,
                   const std::array<uint64_t, 3>& tweak,
                   Threefish3::SecurityMode sec_mode);
    
    /**
     * @brief Process configuration block
     * Initialize hash state with configuration
     */
    static void process_config_block(std::array<uint64_t, Threefish3::NUM_WORDS>& state,
                                   const Config& config);
    
    /**
     * @brief Process message block
     * Core message block processing
     */
    static void process_message_block(std::array<uint64_t, Threefish3::NUM_WORDS>& state,
                                    const uint8_t* block,
                                    size_t block_size,
                                    bool is_last);
    
    /**
     * @brief Process tree node
     * Combine child nodes in tree mode
     */
    static std::vector<uint8_t> process_tree_node(
        const std::vector<std::vector<uint8_t>>& children,
        const Config& config,
        bool is_root);

    static std::vector<uint8_t> checkpoint_data_;
    static bool has_checkpoint_;

    struct BlockContext {
        std::array<uint64_t, Threefish3::NUM_WORDS> state;
        std::array<uint64_t, 3> tweak;
        size_t bytes_processed;
        uint64_t domain;
        bool is_first;
        bool is_final;
        
        BlockContext() : bytes_processed(0), domain(DOMAIN_MSG), 
                        is_first(true), is_final(false) {
            state.fill(0);
            tweak.fill(0);
        }
    };

    static constexpr uint64_t DOMAIN_MSG = 48;
    static constexpr uint64_t DOMAIN_CFG = 28;
    static constexpr uint64_t DOMAIN_OUT = 63;
    static constexpr uint64_t T1_FIRST = 1ULL << 62;
    static constexpr uint64_t T1_FINAL = 1ULL << 63;

    static void process_block(
        BlockContext& ctx,
        const uint8_t* data,
        size_t size,
        Threefish3::SecurityMode sec_mode
    );
};

#endif // SKEIN3_H
