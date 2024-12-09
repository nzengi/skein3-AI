# Skein3 Installation & Usage Guide

## Prerequisites
- C++17 compatible compiler
- CMake 3.10+
- OpenSSL (optional)

## Installation Steps

1. Get your license key
   - Contact us at your@email.com to obtain a license key
   - We will provide you with a unique license key in format: `SKEIN3-XXXXXXX-YYYYYYY`

2. Clone the repository:
```bash
git clone https://github.com/nzengi/skein3-AI.git
cd skein3-AI
```

3. Build the project:
```bash
mkdir build && cd build
cmake ..
make
```

4. Set your license key as an environment variable:
```bash
export SKEIN3_LICENSE="YOUR-PROVIDED-LICENSE-KEY"
```

## Permanent License Setup

To avoid setting the license key every time, add it to your ~/.bashrc:
```bash
echo 'export SKEIN3_LICENSE="YOUR-PROVIDED-LICENSE-KEY"' >> ~/.bashrc
source ~/.bashrc
```

## Usage Examples

### Basic Hashing
```cpp
#include <skein3.h>

std::vector<uint8_t> message = {1, 2, 3, 4, 5};
Skein3::Config config;
config.size = Skein3::HashSize::HASH_512;

auto hash = Skein3::hash(message, config);
```

### Parallel Processing
```cpp
Skein3::Config config;
config.mode = Skein3::HashMode::TREE;
config.tree_fan_out = 8;  // Use 8 threads
auto hash = Skein3::tree_hash(large_data, config);
```

### AI-Optimized Mode
```cpp
Skein3::Config config;
config.neural_config.enable_neural_adaptation = true;
config.neural_config.complexity_threshold = 0.85f;
auto hash = Skein3::hash(data, config);
```

## Support

For any issues or questions:
- Email: your@email.com
- GitHub Issues: https://github.com/nzengi/skein3-AI/issues

## License Types

1. Basic Commercial License
   - Standard features
   - Single developer use

2. Enterprise License
   - All features including quantum resistance
   - Multiple developer use
   - Priority support

Contact your@email.com for licensing information.