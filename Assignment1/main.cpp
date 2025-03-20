#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "encoder.hpp"
#include "decoder.hpp"

int main(int argc, char* argv[]) {
    // Checking command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-e] [-d]\n";
        std::cerr << "  -e: Encode input to ASCII85 (default)\n";
        std::cerr << "  -d: Decode ASCII85 input\n";
        return 1;
    }

    // Determination of the operating mode (encoding or decoding)
    std::string mode = "encode"; 
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-e") {
            mode = "encode";
        } else if (arg == "-d") {
            mode = "decode";
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    // Reading from STDIN and writing in STDOUT
    if (mode == "encode") {
        encoder(std::cin, std::cout);
    } else if (mode == "decode") {
        try {
            decoder(std::cin, std::cout);
        } catch (const std::runtime_error& e) {
            std::cerr << "Decoding error: " << e.what() << "\n";
            return 1; 
        }
    }

    return 0;
}
