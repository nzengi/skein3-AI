# API Reference

## Core Classes

### Skein3
Main hash function implementation with advanced features.

#### Enums
```cpp
enum class HashSize {
    HASH_256 = 256,    // Standard security level
    HASH_512 = 512,    // Enhanced security level
    HASH_1024 = 1024   // Quantum-resistant security level
};

enum class HashMode {
    STANDARD,       // Basic sequential processing
    TREE,          // Parallel tree-based processing
    STREAMING      // Optimized for continuous data streams
};

enum class OptimizationMode {
    AI_TRAINING,      // AI model training optimization
    BLOCKCHAIN,       // Blockchain specific optimization
    STANDARD         // Standard mode
};

enum class MemoryProtectionMode {
    STANDARD,
    QUANTUM_RESISTANT,
    HARDWARE_BACKED
};
```

#### Configurations
```cpp
struct NeuralConfig {
    bool enable_neural_adaptation = false;
    float complexity_threshold = 0.75f;
    size_t adaptation_rounds = 1000;
    std::vector<float> weights;
};

struct Config {
    HashSize size = HashSize::HASH_512;
    HashMode mode = HashMode::STANDARD;
    OptimizationMode opt_mode = OptimizationMode::STANDARD;
    size_t tree_leaf_size = 1024;
    size_t tree_fan_out = 8;
    bool personalization = false;
    std::vector<uint8_t> person_string;
    
    // AI features
    bool batch_processing = false;
    size_t batch_size = 1024;
    bool gpu_acceleration = false;
    
    // Blockchain features
    bool merkle_tree = false;
    bool zero_knowledge = false;
    size_t nonce_space = 32;
    
    // Memory protection
    MemoryProtectionMode mem_protection = MemoryProtectionMode::STANDARD;
    bool secure_memory_wipe = true;
    size_t memory_protection_rounds = 3;
    
    NeuralConfig neural_config;
};
```

#### Core Functions
```cpp
// Basic hash computation
static std::vector<uint8_t> hash(
    const std::vector<uint8_t>& message,
    const Config& config = Config()
);

// Tree-based parallel hashing
static std::vector<uint8_t> tree_hash(
    const std::vector<uint8_t>& message,
    const Config& config = Config()
);

// Batch processing for multiple inputs
static std::vector<std::vector<uint8_t>> batch_hash(
    const std::vector<std::vector<uint8_t>>& messages,
    const Config& config = Config()
);

// GPU-accelerated hash computation
#ifdef CUDA_ENABLED
static std::vector<uint8_t> gpu_hash(
    const std::vector<uint8_t>& message,
    const Config& config = Config()
);
#endif
```

### Security Classes

#### AIModelProtection
```cpp
class AIModelProtection {
public:
    static std::vector<uint8_t> protectModel(
        const std::vector<float>& weights,
        const std::vector<uint8_t>& architecture
    );
    
    static bool updateWeightsSecurely(
        std::vector<float>& weights,
        const std::vector<uint8_t>& updates
    );
};
```

#### SecurityMonitor
```cpp
class SecurityMonitor {
public:
    struct SecurityMetrics {
        float entropy_level;
        float pattern_complexity;
        float attack_probability;
        std::vector<ThreatIndicator> threats;
    };
    
    static SecurityMetrics analyzeHashOperation(
        const std::vector<uint8_t>& input,
        const std::vector<uint8_t>& output
    );
};
```

#### AdaptiveSecurity
```cpp
class AdaptiveSecurity {
public:
    enum class ThreatLevel {
        LOW,
        MEDIUM,
        HIGH,
        CRITICAL
    };
    
    static void adjustSecurityParameters(
        Skein3::Config& config,
        ThreatLevel threat_level
    );
    
    static ThreatLevel analyzeThreatLevel(
        const std::vector<uint8_t>& data,
        const SecurityMetrics& metrics
    );
};
```

### Blockchain Features
```cpp
struct BlockchainFeatures {
    static std::vector<uint8_t> generateZKProof(
        const std::vector<uint8_t>& data,
        const std::vector<uint8_t>& witness
    );
    
    static std::vector<uint8_t> optimizedMerkleRoot(
        const std::vector<std::vector<uint8_t>>& transactions
    );
    
    static bool verifySmartContract(
        const std::vector<uint8_t>& contract_code,
        const std::vector<uint8_t>& state
    );
};
```

### Memory Management
```cpp
class QuantumResistantMemory {
public:
    struct ProtectionConfig {
        ProtectionLevel level = ProtectionLevel::STANDARD;
        size_t rounds = 3;
        bool secure_wipe = true;
        size_t memory_fence_size = 64;
    };

    template<typename T>
    static std::unique_ptr<T[]> allocateSecure(
        size_t size,
        const ProtectionConfig& config
    );

    template<typename T>
    static bool verifyIntegrity(
        const T* memory,
        size_t size,
        const ProtectionConfig& config
    );
};
```

## Error Handling

### Exceptions
```cpp
class LicenseException : public std::runtime_error {
    // Thrown for license validation failures
};

class SecurityException : public std::runtime_error {
    // Thrown for security violations
};

class ConfigurationException : public std::runtime_error {
    // Thrown for invalid configurations
};
```

## Thread Management
```cpp
class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    
    template<class F>
    std::future<void> enqueue(F&& f);
};
``` 