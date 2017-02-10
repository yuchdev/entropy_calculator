#include <entropy_util//command_line_parser.h>

#include <string>
#include <list>
#include <iostream>
#include <algorithm>

using namespace entropy;
namespace po = boost::program_options;
using std::string;

CommandLineParams::CommandLineParams() : cmd_options_description("Shannon-entropy utility console has the following options:")
{}

void CommandLineParams::read_params(int argc, char* argv[])
{
    cmd_options_description.add_options()
        ("help,h", "Print usage")
        ("version,v", "Print version")
        ("from-file,f", po::value<string>(&_from_file), "Get a file as an information source (default option)")
        ("random-distribution,r", po::value<string>(&_random_distribution),
            "Get a random distribution as information source [linear|normal]")
        ("sequence-size,s", po::value<size_t>(&_sequence_size),
            "Size of the generated sequence (only if --random-distribution selected)")
        ("mean,m", po::value<double>(&_mean)->default_value(0.), "Mean for distribution (only for normal)")
        ("std-dev,d", po::value<double>(&_stddev)->default_value(1.0), "Standard deviation for distribution (only for normal)")
        ;

    // command line params processing
    po::variables_map cmd_variables_map;
    po::store(parse_command_line(argc, argv, cmd_options_description), cmd_variables_map);
    po::notify(cmd_variables_map);

    set_flag(cmd_variables_map, _help, "help");
    set_flag(cmd_variables_map, _version, "version");

    // do not check debug flags!
    std::list<bool> mutually_exclusives = { _help, _version, !_from_file.empty(), !_random_distribution.empty() };
    size_t options_count = std::count(mutually_exclusives.begin(), mutually_exclusives.end(), true);
    if (options_count > 1) {
        throw std::logic_error("Incompatible command line parameters set, use only one");
    }

    std::list<bool> must_be_together = { !_random_distribution.empty(), _sequence_size ? true : false };
    options_count = std::count(must_be_together.begin(), must_be_together.end(), true);
    if ((options_count) && (options_count != 2)) {
        throw std::logic_error("Sequence size should be set with the generated distribution");
    }

}

/**@brief Set some logical param */
void CommandLineParams::set_flag(const po::variables_map& vm, bool& flag, const char* str)
{
    if (vm.count(str)) {
        flag = true;
    }
}
