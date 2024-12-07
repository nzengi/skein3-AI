#ifndef SKEIN3_H
#define SKEIN3_H

// Standard library includes
#include <vector>
#include <array>
#include <cstdint>
#include <memory>

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
    // Hash output size options
    enum class HashSize {
        HASH_256 = 256,    // Standard security level
        HASH_512 = 512,    // Enhanced security level
        HASH_1024 = 1024   // Quantum-resistant security level
    };

    // Hash operation modes
    enum class HashMode {
        STANDARD,       // Basic sequential processing
        TREE,          // Parallel tree-based processing
        STREAMING      // Optimized for continuous data streams
    };

    // Optimization modes
    enum class OptimizationMode {
        AI_TRAINING,      // AI model training optimization
        BLOCKCHAIN,       // Blockchain specific optimization
        STANDARD         // Standard mode
    };

    // Memory protection modes
    enum class MemoryProtectionMode {
        STANDARD,
        QUANTUM_RESISTANT,
        HARDWARE_BACKED
    };

    // Neural network based hash adaptation
    struct NeuralConfig {
        bool enable_neural_adaptation = false;
        float complexity_threshold = 0.75f;
        size_t adaptation_rounds = 1000;
        std::vector<float> weights;
    };

    // Configuration options
    struct Config {
        HashSize size = HashSize::HASH_512;
        HashMode mode = HashMode::STANDARD;
        OptimizationMode opt_mode = OptimizationMode::STANDARD;
        size_t tree_leaf_size = 1024;  // Size of leaf nodes in tree mode
        size_t tree_fan_out = 8;       // Number of child nodes per parent
        bool personalization = false;   // Enable personalization string
        std::vector<uint8_t> person_string; // Custom personalization data
        
        // AI features
        bool batch_processing = false;    // Batch hash processing for AI
        size_t batch_size = 1024;        // Size of batches
        bool gpu_acceleration = false;    // GPU acceleration support
        
        // Blockchain features
        bool merkle_tree = false;        // Merkle tree support
        bool zero_knowledge = false;     // Zero-knowledge proof support
        size_t nonce_space = 32;        // Nonce space for mining
        
        // Memory protection features
        MemoryProtectionMode mem_protection = MemoryProtectionMode::STANDARD;
        bool secure_memory_wipe = true;
        size_t memory_protection_rounds = 3;

        NeuralConfig neural_config;
    };

    /**
     * @brief Compute hash of input message
     * @param message Input data to be hashed
     * @param config Hash configuration options
     * @return Hash value as byte vector
     */
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& message,
                                   const Config& config = Config()) {
        checkLicense(config);
        
        // Determine security mode
        Threefish3::SecurityMode sec_mode;
        switch (config.size) {
            case HashSize::HASH_1024:
                sec_mode = Threefish3::SecurityMode::QUANTUM;
                break;
            case HashSize::HASH_512:
                sec_mode = Threefish3::SecurityMode::ENHANCED;
                break;
            default:
                sec_mode = Threefish3::SecurityMode::STANDARD;
        }

        // Initialize state
        std::array<uint64_t, Threefish3::NUM_WORDS> state;
        state.fill(0);
        
        // Process configuration block
        process_config_block(state, config);
        
        // Process message blocks
        BlockContext ctx;
        ctx.state = state;
        
        const size_t block_size = Threefish3::BLOCK_SIZE;
        size_t remaining = message.size();
        size_t offset = 0;
        
        while (remaining > 0) {
            size_t current_size = std::min(remaining, block_size);
            bool is_last = (remaining <= block_size);
            
            if (is_last) {
                ctx.is_final = true;
            }
            
            process_block(ctx, 
                         message.data() + offset, 
                         current_size,
                         sec_mode);
            
            offset += current_size;
            remaining -= current_size;
        }
        
        // Output transformation
        BlockContext out_ctx;
        out_ctx.state = ctx.state;
        out_ctx.domain = DOMAIN_OUT;
        out_ctx.is_final = true;
        
        std::array<uint64_t, Threefish3::NUM_WORDS> output_block = {};
        process_block(out_ctx,
                     reinterpret_cast<const uint8_t*>(output_block.data()),
                     block_size,
                     sec_mode);
        
        // Generate final hash
        size_t hash_size = static_cast<size_t>(config.size) / 8;
        std::vector<uint8_t> result(hash_size);
        std::memcpy(result.data(), out_ctx.state.data(), hash_size);
        
        return result;
    }
    
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
};

#endif // SKEIN3_H
