#include <entropy_calculator/random_distributions.h>
#include <iostream>
#include <random>
#include <functional>
#include <algorithm>


std::vector<uint8_t> entropy::generate_uniform_distribution(size_t sequence_size)
{
    std::vector<uint8_t> random_sequence;
    std::random_device rnd_device;

    std::cout << "Random device provided entropy: " << rnd_device.entropy() << '\n';

    std::mt19937 mersenne_engine(rnd_device());
    std::uniform_int_distribution<unsigned> dist(0, 255);

    auto gen = std::bind(dist, mersenne_engine);
    random_sequence.resize(sequence_size);
    std::generate(random_sequence.begin(), random_sequence.end(), gen);
    return std::move(random_sequence);
}

std::vector<uint8_t> entropy::generate_normal_distribution(size_t sequence_size, double mean, double std_dev)
{
    std::vector<uint8_t> random_sequence;
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(mean, std_dev);

    auto gen = std::bind(distribution, generator);

    random_sequence.resize(sequence_size);
    std::generate(random_sequence.begin(), random_sequence.end(), [&gen]() { 
        return static_cast<uint8_t>(gen() * 255); 
    });
    return std::move(random_sequence);
}
