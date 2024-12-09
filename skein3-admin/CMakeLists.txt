cmake_minimum_required(VERSION 3.10)
project(skein3_admin)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(license_generator tools/license/license_generator.cpp)

# Link with required libraries
target_link_libraries(license_generator PRIVATE skein3_lib) 