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
endif()

# Add source files
set(SOURCES
    src/threefish3.cpp
    src/skein3.cpp
    src/license.cpp
    src/thread_pool.cpp
    src/base64.cpp
    src/ai_protection.cpp
    src/blockchain_features.cpp
    src/security_monitor.cpp
    src/adaptive_security.cpp
    src/performance_metrics.cpp
)

# First, create the library
add_library(skein3_lib STATIC ${SOURCES})

# Then, create the main executable
add_executable(skein3_main main.cpp)

# Link the library to the main executable
target_link_libraries(skein3_main PRIVATE skein3_lib)

# Optional: Link OpenSSL if found
if(OPENSSL_FOUND)
    target_link_libraries(skein3_lib PRIVATE ${OPENSSL_LIBRARIES})
endif()

# Test targets
add_executable(basic_test examples/basic_test.cpp)
add_executable(performance_test examples/performance_test.cpp)
add_executable(parallel_test examples/parallel_test.cpp)
add_executable(security_test examples/security_test.cpp)
add_executable(comprehensive_test examples/comprehensive_test.cpp)
add_executable(customization_test examples/customization_test.cpp)
add_executable(error_handling_test examples/error_handling_test.cpp)
add_executable(integration_test examples/integration_test.cpp)
add_executable(license_test examples/license_test.cpp)
add_executable(memory_test examples/memory_test.cpp)
add_executable(performance_metrics_test examples/performance_metrics_test.cpp)
add_executable(security_benchmark examples/security_benchmark.cpp)
add_executable(readme_test readme_test.cpp)

# Link test targets with the library
target_link_libraries(basic_test PRIVATE skein3_lib)
target_link_libraries(performance_test PRIVATE skein3_lib)
target_link_libraries(parallel_test PRIVATE skein3_lib)
target_link_libraries(security_test PRIVATE skein3_lib)
target_link_libraries(comprehensive_test PRIVATE skein3_lib)
target_link_libraries(customization_test PRIVATE skein3_lib)
target_link_libraries(error_handling_test PRIVATE skein3_lib)
target_link_libraries(integration_test PRIVATE skein3_lib)
target_link_libraries(license_test PRIVATE skein3_lib)
target_link_libraries(memory_test PRIVATE skein3_lib)
target_link_libraries(performance_metrics_test PRIVATE skein3_lib)
target_link_libraries(security_benchmark PRIVATE skein3_lib)
target_link_libraries(readme_test PRIVATE skein3_lib)

# CTest integration
enable_testing()
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME PerformanceTest COMMAND performance_test)
add_test(NAME ParallelTest COMMAND parallel_test)
add_test(NAME SecurityTest COMMAND security_test)
add_test(NAME ComprehensiveTest COMMAND comprehensive_test)
add_test(NAME CustomizationTest COMMAND customization_test)
add_test(NAME ErrorHandlingTest COMMAND error_handling_test)
add_test(NAME IntegrationTest COMMAND integration_test)
add_test(NAME LicenseTest COMMAND license_test)
add_test(NAME MemoryTest COMMAND memory_test)
add_test(NAME PerformanceMetricsTest COMMAND performance_metrics_test)
add_test(NAME SecurityBenchmarkTest COMMAND security_benchmark)
add_test(NAME ReadmeTest COMMAND readme_test)

# CUDA support
option(WITH_CUDA "Enable CUDA support" OFF)
if(WITH_CUDA)
    enable_language(CUDA)
    add_definitions(-DCUDA_ENABLED)
    find_package(CUDA REQUIRED)
    include_directories(${CUDA_INCLUDE_DIRS})
endif()

# Install targets
install(TARGETS skein3_main DESTINATION bin)
install(TARGETS skein3_lib DESTINATION lib)
install(FILES 
    include/skein3.h 
    include/threefish3.h 
    include/thread_pool.h
    DESTINATION include/skein3
)
