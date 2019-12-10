#include <entropy/shannon_entropy.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <cassert>

using namespace entropy;
using namespace std;
namespace fs = boost::filesystem;

bool entropy::ShannonEncryptionChecker::interrupt_all_ = false;
bool entropy::ShannonEncryptionChecker::load_uint8_codecvt_;

std::map<ShannonEncryptionChecker::InformationEntropyEstimation, std::string> 
ShannonEncryptionChecker::entropy_string_description_ = {
    { Plain , "Plain"},
    { Binary , "Binary" },
    { Encrypted , "Encrypted" },
    { Unknown , "Unknown" } };

ShannonEncryptionChecker::ShannonEncryptionChecker()
{
    assert(entropy_string_description_.size() == EntropyLevelSize);
    if (false == load_uint8_codecvt_) {
        std::locale::global(std::locale(std::locale(), new std::codecvt<uint8_t, char, std::mbstate_t>));
        load_uint8_codecvt_ = true;
    }
}

void ShannonEncryptionChecker::interrupt()
{
    interrupt_all_ = true;
}

std::string ShannonEncryptionChecker::get_information_description(InformationEntropyEstimation ent) const
{
    std::string descr = entropy_string_description_[ent];
    
    // all descriptions must be provided!
    assert(!descr.empty());
    return descr;
}

size_t ShannonEncryptionChecker::min_compressed_size(double entropy, size_t sequence_size) const
{
    return static_cast<size_t>((entropy * sequence_size) / 8);
}

double entropy::ShannonEncryptionChecker::get_file_entropy(const std::string& file_path) const
{
    uintmax_t file_size = fs::file_size(file_path);
    std::vector<double> byte_probabilities = read_file_probabilities(file_path, file_size);
    return shannon_entropy(byte_probabilities.begin(), byte_probabilities.end());
}


void ShannonEncryptionChecker::set_callback(callback_t callback)
{
    callback_ = callback;
}

double ShannonEncryptionChecker::get_sequence_entropy(const uint8_t* sequence_start, size_t sequence_size) const
{
    std::vector<double> byte_probabilities = read_stream_probabilities(sequence_start, sequence_size);
    return shannon_entropy(byte_probabilities.begin(), byte_probabilities.end());
}

ShannonEncryptionChecker::InformationEntropyEstimation
ShannonEncryptionChecker::information_entropy_estimation(double entropy, size_t sequence_size) const
{
    double epsilon = estimated_epsilon(sequence_size);
    if ((8.0 - entropy) < epsilon) {
        return Encrypted;
    }
    if (entropy > 6.0) {
        return Binary;
    }
    if (entropy >= 0. && entropy <= 6.0) {
        return Plain;
    }
    // should not be here, entropy calculation error
    assert(false);
    return Unknown;
}

std::vector<double> ShannonEncryptionChecker::read_file_probabilities(const std::string& file_path, size_t file_size) const
{
    // probability of every byte of zero-sized file is 0
    if (0 == file_size) {
        return std::vector<double>(256);
    }

    uint8_t read_ahead_buffer[MAX_BUFFER_SIZE];

    std::vector<size_t> bytes_distribution(256);
    std::vector<double> bytes_frequencies(256);
    std::basic_ifstream<uint8_t, std::char_traits<uint8_t>> file(file_path, std::ios::in | std::ios::binary);
    file.rdbuf()->pubsetbuf(read_ahead_buffer, MAX_BUFFER_SIZE);

    uint8_t one_byte{};
    uintmax_t counter{};
    while (!file.eof()) {

        if (interrupt_all_) {
            return std::vector<double>{};
        }

        file.read(reinterpret_cast<uint8_t*>(&one_byte), sizeof(uint8_t));
        if (callback_) {
            callback_(++counter);
        }
        ++bytes_distribution[one_byte];
    }

    for (size_t i = 0; i != 256; ++i) {
        if (interrupt_all_) {
            return std::vector<double>{};
        }

        bytes_frequencies[i] = static_cast<double>(bytes_distribution[i]) / file_size;
    }

    return std::move(bytes_frequencies);
}

std::vector<double> ShannonEncryptionChecker::read_stream_probabilities(const uint8_t* sequence_start, size_t sequence_size) const
{
    if (0 == sequence_size) {
        return std::move(std::vector<double>(256));
    }

    std::vector<size_t> bytes_distribution(256);
    std::vector<double> bytes_frequencies(256);

    uintmax_t counter{};
    for (size_t i = 0; i < sequence_size; ++i) {
        if (interrupt_all_) {
            return std::vector<double>{};
        }

        if (callback_) {
            callback_(++counter);
        }

        ++bytes_distribution[sequence_start[i]];
    }

    for (size_t i = 0; i != 256; ++i) {
        if (interrupt_all_) {
            return std::vector<double>{};
        }

        bytes_frequencies[i] = static_cast<double>(bytes_distribution[i]) / sequence_size;
    }

    return std::move(bytes_frequencies);
}

double ShannonEncryptionChecker::estimated_epsilon(size_t sample_size) const
{
    // Note: numbers based on very approximate estimations (several test calculations)
    // More reliable statistic should be collected for more exact results
    if (sample_size < (1024 * 1024)) {
        return 0.001;
    }
    if (sample_size < (1024 * 1024 * 64)) {
        return 0.0001;
    }
    if (sample_size < (1024 * 1024 * 512)) {
        return 0.00001;
    }
    return 0.000001;
}


