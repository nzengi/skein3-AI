# Skein3 Installation Guide

## Prerequisites
- C++17 compatible compiler
- CMake 3.10+
- OpenSSL (optional)
- CUDA (optional, for GPU support)

## Supported Platforms
- Linux (Ubuntu 20.04+)
- Windows (MSVC 2019+)
- macOS (Clang 10+)

## Build from Source

### Linux/macOS
```bash
git clone https://github.com/nzengi/skein3.git
cd skein3
mkdir build && cd build
cmake ..
make
sudo make install
```

### Windows (Visual Studio)
1. Install Visual Studio 2019 or later
2. Install CMake
3. Open Developer Command Prompt
```cmd
git clone https://github.com/nzengi/skein3.git
cd skein3
mkdir build && cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

## Optional Features

### GPU Support
```bash
cmake -DWITH_CUDA=ON ..
```

### Custom Installation
```bash
cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..
```

## Linking in Your Project
```cmake
find_package(Skein3 REQUIRED)
target_link_libraries(your_project Skein3::skein3)
```