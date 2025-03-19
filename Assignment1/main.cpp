#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "encoder.hpp"  
#include "decoder.hpp"  

int main() {
    std::string input;
    std::cin >> input;

    std::ifstream infile(input, std::ios::binary);
    if (!infile) {  
        std::cerr << "Problems with reading file\n";
        return 1;
    }

    auto f_size = std::filesystem::file_size(input);
    std::vector<char> buffer(f_size);  

    infile.read(buffer.data(), f_size);
    infile.close();

    short n;
    std::cout << "Choose encoder(1) or decoder(2): ";	
    std::cin >> n;

    std::ostringstream output;  

    switch (n) {
        case 1: {
            std::istringstream input_stream(std::string(buffer.begin(), buffer.end()));
            encoder(input_stream, output);
            break;
        }
        case 2: {
            std::istringstream input_stream(std::string(buffer.begin(), buffer.end()));
            decoder(input_stream, output);
            break;
        }
        default:
            std::cerr << "Invalid choice.\n";
            return 1;
    }

    std::ofstream outfile("new_file.txt", std::ios::binary);
    if (!outfile) {  
        std::cerr << "Problems with writing file\n";
        return 1;
    }

    std::string result = output.str();
    outfile.write(result.data(), result.size());
    outfile.close();

    std::cout << "Operation completed successfully!\n";
    return 0;
}

