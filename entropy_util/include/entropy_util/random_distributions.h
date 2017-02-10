#pragma once
#include <cstdint>
#include <vector>

namespace entropy {

std::vector<uint8_t> generate_uniform_distribution(size_t sequence_size);

std::vector<uint8_t> generate_normal_distribution(size_t sequence_size, double mean, double std_dev);

} // namespace entropy
