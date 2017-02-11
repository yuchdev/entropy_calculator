#pragma once
#include <string>
#include <boost/program_options.hpp>

// The header contains command-line parser
// It uses Boost.ProgramOptions

namespace entropy {

/// @brief Storage for all passed and default command-line params
class CommandLineParams {
public:

    /**@brief Should have a constructor by default */
    CommandLineParams();

    /**@brief Parse raw command-line parameters */
    void read_params(int argc, char* argv[]);

    /**@brief Set some logical param */
    void set_flag(const boost::program_options::variables_map& vm, bool& flag, const char* str);

    /**@brief Get available options list*/
    boost::program_options::options_description& options_descript(){ return cmd_options_description; }

    // Accessors

    bool is_help() const{
        return _help;
    }

    bool is_version() const {
        return _version;
    }

    const std::string& read_from_file() const {
        return _from_file;
    }

    const std::string& random_distribution() const {
        return _random_distribution;
    }

    double mean() const {
        return _help;
    }

    double std_deviation() const {
        return _stddev;
    }

    size_t sequence_size() const {
        return _sequence_size;
    }

private:

    /// Show help
    bool _help = false;

    /// Show version
    bool _version = false;

    /// Read file
    std::string _from_file;

    /// Mean
    double _mean = 0.;

    /// Standard deviation
    double _stddev = 0.;

    /// Generates sequence size
    size_t _sequence_size = 0;

    /// Generate sequence from random generator with provided distribution
    std::string _random_distribution;

    /// Command-line params description
    boost::program_options::options_description cmd_options_description;
};

} // namespace entropy
