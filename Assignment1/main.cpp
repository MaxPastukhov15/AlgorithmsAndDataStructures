#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

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

    std::ofstream outfile("new_file.txt", std::ios::binary);
    if (!outfile) {  
        std::cerr << "Problems with writing file\n";
        return 1;
    }

    outfile.write(buffer.data(), f_size);
    outfile.close();

    return 0;
}

