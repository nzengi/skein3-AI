# Skein3 API Reference

## Core Hash Function
```cpp
static std::vector<uint8_t> hash(
    const std::vector<uint8_t>& message,
    const Config& config = Config()
);
```

## Streaming Hash
```cpp
class StreamingHasher {
public:
    StreamingHasher(const Config& config = Config());
    void update(const std::vector<uint8_t>& data);
    std::vector<uint8_t> finalize();
};
```

## Configuration Options
```cpp
struct Config {
    HashSize size{HashSize::HASH_512};
    HashMode mode{HashMode::STANDARD};
    OptimizationMode opt_mode{OptimizationMode::STANDARD};
    
    // Neural network configuration
    NeuralConfig neural_config;
    
    // Memory protection
    MemoryProtectionMode mem_protection{MemoryProtectionMode::STANDARD};
};
```

## Advanced Methods
- `mac()`: Message Authentication Code
- `tree_hash()`: Parallel tree-based hashing
- `batch_hash()`: Process multiple messages
- `merkle_root()`: Blockchain-specific root computation
- `verify_zero_knowledge()`: Zero-knowledge proof verification

## Memory Security
```cpp
template<typename T>
static std::unique_ptr<T[]> allocateSecureMemory(
    size_t size,
    const Config& config
);
```

## Neural Adaptation
```cpp
static void initializeNeuralAdapter(const Config& config);
static void saveNeuralWeights(const std::string& filename);
static void loadNeuralWeights(const std::string& filename);
``` 