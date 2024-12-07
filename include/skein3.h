#ifndef SKEIN3_H
#define SKEIN3_H

#include <vector>
#include <array>
#include <cstdint>
#include "threefish3.h"

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

    // Configuration options
    struct Config {
        HashSize size = HashSize::HASH_512;
        HashMode mode = HashMode::STANDARD;
        size_t tree_leaf_size = 1024;  // Size of leaf nodes in tree mode
        size_t tree_fan_out = 8;       // Number of child nodes per parent
        bool personalization = false;   // Enable personalization string
        std::vector<uint8_t> person_string; // Custom personalization data
    };

    /**
     * @brief Compute hash of input message
     * @param message Input data to be hashed
     * @param config Hash configuration options
     * @return Hash value as byte vector
     */
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& message,
                                   const Config& config = Config());
    
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

private:
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
