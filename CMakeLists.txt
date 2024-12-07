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

# Önce kütüphaneyi oluştur
add_library(skein3_lib ${SOURCES})

# Ana uygulamayı oluştur
add_executable(skein3 main.cpp)
target_link_libraries(skein3 skein3_lib)

# Test hedeflerini ekle
add_executable(basic_test examples/basic_test.cpp)
add_executable(performance_test examples/performance_test.cpp)
add_executable(parallel_test examples/parallel_test.cpp)
add_executable(security_test examples/security_test.cpp)

# Test hedeflerini bağla
target_link_libraries(basic_test skein3_lib)
target_link_libraries(performance_test skein3_lib)
target_link_libraries(parallel_test skein3_lib)
target_link_libraries(security_test skein3_lib)

# CTest entegrasyonu
enable_testing()
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME PerformanceTest COMMAND performance_test)
add_test(NAME ParallelTest COMMAND parallel_test)
add_test(NAME SecurityTest COMMAND security_test)

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
