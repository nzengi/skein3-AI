cmake_minimum_required(VERSION 3.10)
project(skein3_project)

set(CMAKE_CXX_STANDARD 17)

# Compiler flags for SIMD optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4.2 -O3")
    
    # Enable additional warnings and security flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fstack-protector-strong")
    
    # Enable Link Time Optimization
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /O2")
    
    # Enable additional warnings and security flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /GS")
    
    # Enable Link Time Optimization
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
endif()

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add OpenSSL if needed (optional)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(skein3 ${OPENSSL_LIBRARIES})
endif()

# Add source files
set(SOURCES
    src/threefish3.cpp
    src/skein3.cpp
    src/thread_pool.cpp
)

# Create main executable
add_executable(skein3 main.cpp ${SOURCES})

# Create test executable
add_executable(tests tests/test_skein3.cpp ${SOURCES})

# Enable testing
enable_testing()
add_test(NAME SkeinTests COMMAND tests)

# Install targets
install(TARGETS skein3 DESTINATION bin)
install(FILES include/skein3.h include/threefish3.h DESTINATION include)

# CUDA support
option(WITH_CUDA "Enable CUDA support" OFF)
if(WITH_CUDA)
    enable_language(CUDA)
    add_definitions(-DCUDA_ENABLED)
    find_package(CUDA REQUIRED)
    include_directories(${CUDA_INCLUDE_DIRS})
endif()
