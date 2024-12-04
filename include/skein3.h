#ifndef SKEIN3_H
#define SKEIN3_H

#include <vector>
#include <array>
#include <cstdint>

std::vector<uint8_t> skein3_hash(const std::vector<uint8_t>& message, size_t hash_size = 64);

#endif // SKEIN3_H
