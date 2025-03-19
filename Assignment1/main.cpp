#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "encoder.hpp"
#include "decoder.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " encode|decode input_file [output_file|--stdout]\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file = (argc >= 4 && std::string(argv[3]) != "--stdout") ? argv[3] : "";

    bool to_stdout = (argc >= 4 && std::string(argv[3]) == "--stdout");

    std::ifstream infile(input_file, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    std::ostream* output_stream = &std::cout;
    std::ofstream outfile;
    if (!to_stdout) {
        if (output_file.empty()) output_file = "output.txt";
        outfile.open(output_file, std::ios::binary);
        if (!outfile) {
            std::cerr << "Error opening output file.\n";
            return 1;
        }
        output_stream = &outfile;
    }

    char buffer[1024];
    while (infile.read(buffer, sizeof(buffer)) || infile.gcount() > 0) {
        std::string data(buffer, infile.gcount());
        std::istringstream input_stream(data);
        std::ostringstream encoded_output;

        if (mode == "encode") {
            encoder(input_stream, encoded_output);
        } else if (mode == "decode") {
            decoder(input_stream, encoded_output);
        } else {
            std::cerr << "Invalid mode. Use 'encode' or 'decode'.\n";
            return 1;
        }

        *output_stream << encoded_output.str();
    }

    infile.close();
    if (outfile.is_open()) outfile.close();

    return 0;
}

