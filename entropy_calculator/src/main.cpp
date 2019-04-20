#include <entropy/shannon_entropy.h>
#include <entropy_calculator/random_distributions.h>
#include <entropy_calculator/command_line_parser.h>

#include <boost/progress.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

using namespace std;
using namespace entropy;
using binary_file = std::basic_ifstream<uint8_t, std::char_traits<uint8_t>>;

struct progress_cb 
{
    progress_cb() = default;

    void init(unsigned long op_count)
    {
        progress_bar = std::make_unique<boost::progress_display>(
            static_cast<unsigned long>(op_count), 
            std::cout, 
            "\n%\t ", 
            "\t ", 
            "Complete:");
    }

    void operator()()
    {
        ++(*progress_bar);
    }

    uintmax_t count;
    std::shared_ptr<boost::progress_display> progress_bar;
};


static progress_cb& get_progress()
{
    static progress_cb pd;
    return pd;
}

void progress_callback(uintmax_t iteration)
{
    auto progress = get_progress();
    progress();
}

static CommandLineParams& get_params()
{
    static CommandLineParams p;
    return p;
}

#if defined(_WIN32) || defined(_WIN64)

BOOL WINAPI ctrl_handler(DWORD ctrl_type)
{
    // Ctrl Event types
    // https://msdn.microsoft.com/en-us/library/ms683242(v=vs.85).aspx
    ShannonEncryptionChecker::interrupt();
    return TRUE;
}

#endif

void usage_exit()
{
    cout << get_params().options_descript() << endl;
    exit(EXIT_SUCCESS);
}


void print_version_exit()
{
    cout << "0.0.3" << endl;
    exit(EXIT_SUCCESS);
}

void calculate_file_entropy(const std::string& filename) 
{
    std::cout << "Please patience, entropy calculation on big files takes a while...\n";
    auto start = chrono::steady_clock::now();

    ShannonEncryptionChecker shannon;
    
    size_t file_size = ShannonEncryptionChecker::get_file_size(filename);

    get_progress().init(file_size);
    shannon.set_callback(&progress_callback);

    double entropy = shannon.get_file_entropy(filename);
    size_t min_compressed = shannon.min_compressed_size(entropy, file_size);
    ShannonEncryptionChecker::InformationEntropyEstimation entropy_estimation = 
        shannon.information_entropy_estimation(entropy, file_size);
    auto end = chrono::steady_clock::now();
    std::string description = shannon.get_information_description(entropy_estimation);

    auto diff = end - start;
    std::cout << "File name: " << filename << '\n';
    std::cout << "File size = " << file_size << " bytes\n";
    std::cout << "Entropy = " << std::setprecision(16) << entropy << '\n';
    std::cout << "Time = " << static_cast<int>(chrono::duration<double, milli>(diff).count()) << " ms" << '\n';
    std::cout << "Information entropy estimation: " << description << '\n';
    std::cout << "Min possible file size assuming max theoretical compression efficiency: " << min_compressed << " bytes\n";
}

void calculate_sequence_entropy(const std::vector<uint8_t>& sequence)
{
    std::cout << "Please patience, entropy calculation on big files takes a while...\n";
    ShannonEncryptionChecker shannon;
    double entropy = shannon.get_sequence_entropy(&sequence[0], sequence.size());
    size_t min_compressed = shannon.min_compressed_size(entropy, sequence.size());
    ShannonEncryptionChecker::InformationEntropyEstimation entropy_estimation = shannon.information_entropy_estimation(entropy, sequence.size());
    std::string description = shannon.get_information_description(entropy_estimation);

    std::cout << "Sequence size = " << sequence.size() << " bytes\n";
    std::cout << "Entropy = " << std::setprecision(16) << entropy << '\n';
    std::cout << "Information entropy estimation: " << description << '\n';
    std::cout << "Min possible file size assuming max theoretical compression efficiency: " << min_compressed << " bytes\n";
}

int main(int argc, char* argv[]) {

    setlocale(0, "");

    if (argc == 1) {
        usage_exit();
    }

    try {
        get_params().read_params(argc, argv);
        const CommandLineParams& cmd_line_params = get_params();

        if (cmd_line_params.is_help()) {
            usage_exit();
        }

        if (cmd_line_params.is_version()) {
            print_version_exit();
        }

        if (!cmd_line_params.read_from_file().empty()) {
            calculate_file_entropy(cmd_line_params.read_from_file());
            return EXIT_SUCCESS;
        }

        if (!cmd_line_params.random_distribution().empty()) {
            std::string distr = cmd_line_params.random_distribution();
            if (distr == "normal") {
                size_t sequence_size = cmd_line_params.sequence_size();
                std::vector<uint8_t> random_sequence = generate_uniform_distribution(sequence_size);
                calculate_sequence_entropy(random_sequence);
            }
            if (distr == "linear") {
                size_t sequence_size = cmd_line_params.sequence_size();
                double mean = cmd_line_params.mean();
                double std_dev = cmd_line_params.std_deviation();
                std::vector<uint8_t> random_sequence = entropy::generate_normal_distribution(sequence_size, mean, std_dev);
                calculate_sequence_entropy(random_sequence);
            }
            else {
                usage_exit();
            }
        }

    }
    // boost::program_options exception reports
    // about wrong command line parameters usage
    catch (const boost::program_options::error& e) {
        cout << "Program option error: " << e.what() << endl;
        cout << get_params().options_descript() << endl;
        usage_exit();
    }
    catch (const std::exception& e) {
        cout << "General exception: " << e.what() << endl;
        return EXIT_SUCCESS;
    }

    if (argc == 2) {

        const std::string filename(argv[1]);
        calculate_file_entropy(filename);
    }


    return 0;
}
