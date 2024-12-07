cmake_minimum_required(VERSION 3.10)
project(skein3_project)

set(CMAKE_CXX_STANDARD 17)

# SIMD optimizasyonları için derleyici flagleri
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4.2")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
endif()

# Kaynak dosyaları ekle
include_directories(include)
add_executable(skein3 main.cpp src/threefish3.cpp src/skein3.cpp)
add_executable(tests tests/test_skein3.cpp src/threefish3.cpp src/skein3.cpp)
