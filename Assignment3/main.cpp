#include <iostream>
#include <string>
#include <iomanip>
#include "Shannon.hpp"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Error: Invalid number of arguments\n";
        std::cerr << "Usage: ./shannon [-e|--encode|-d|--decode] input_file output_file dictionary_file\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string dictionaryFile = argv[4];

    try {
        ShannonCodec codec;
        codec.loadDictionary(dictionaryFile);

        if (mode == "-e" || mode == "--encode") {
            std::cout << "Encoding " << inputFile << "...\n";
            codec.encode(inputFile, outputFile);

            // Calculate compression ratio
            std::ifstream in(inputFile, std::ios::binary | std::ios::ate);
            std::ifstream out(outputFile, std::ios::binary | std::ios::ate);

            if (!in || !out) {
                throw std::runtime_error("Cannot calculate compression ratio");
            }

            size_t originalSize = in.tellg();
            size_t compressedSize = out.tellg();
            double ratio = (1.0 - (double)compressedSize / originalSize) * 100.0;

            std::cout << "Encoding complete!\n";
            std::cout << "Original size: " << originalSize << " bytes\n";
            std::cout << "Compressed size: " << compressedSize << " bytes\n";
            std::cout << "Compression ratio: " << std::fixed << std::setprecision(2) << ratio << "%\n";

        } else if (mode == "-d" || mode == "--decode") {
            std::cout << "Decoding " << inputFile << "...\n";
            codec.decode(inputFile, outputFile);
            std::cout << "Decoding complete!\n";

        } else {
            std::cerr << "Error: Invalid mode specified\n";
            std::cerr << "Usage: ./shannon [-e|--encode|-d|--decode] input_file output_file dictionary_file\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

