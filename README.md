# Skein3: Advanced Cryptographic Hash Function

Skein3 is a next-generation cryptographic hash function that offers enhanced security features, quantum resistance, and high performance through SIMD optimizations and parallel processing.

## Key Features

- 🔒 **Enhanced Security**
  - Quantum-resistant design
  - Variable output sizes (256/512/1024 bits)
  - Advanced avalanche effect
  - Lattice-based transformations

- ⚡ **High Performance**
  - SIMD optimizations (AVX2/SSE4.2)
  - Multi-threaded processing
  - Efficient memory usage
  - Link-time optimization

- 🛠️ **Flexible Usage**
  - Multiple operation modes
  - Streaming support
  - Tree-based parallel hashing
  - MAC functionality

## Security Features

- **Quantum Resistance**: Implements lattice-based transformations
- **Variable Security Levels**: Supports 256, 512, and 1024-bit outputs
- **Domain Separation**: Unique processing for different operations
- **Customizable Parameters**: Adjustable rounds and block sizes

## Performance

| Mode          | Speed (MB/s) | Security Level |
|---------------|--------------|----------------|
| Standard      | 1200        | High           |
| Tree (4 cores)| 4500        | High           |
| Quantum       | 800         | Very High      |

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- AVX2 supported CPU (for SIMD optimizations)

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

### Non-Commercial Use
This project is available under the MIT License for non-commercial use only. See the [LICENSE](LICENSE) file for details.

### Commercial Use
For commercial use, please contact nzengi@github.com for licensing options. Commercial licenses include:

- **Basic Commercial License**
  - Single product use
  - Technical support
  - Security updates

- **Enterprise License**
  - Multiple product use
  - Priority support
  - Security updates
  - Custom feature development

### Ownership and Rights
All rights reserved © 2024 nzengi. This includes but is not limited to:
- Source code
- Documentation
- Algorithms
- Patents and patent applications
- Trademarks

Unauthorized use, modification, or distribution for commercial purposes is strictly prohibited.

## Contact

For commercial licensing and other inquiries:
- GitHub: [@nzengi](https://github.com/nzengi)
