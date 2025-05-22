#pragma once

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <queue>
#include <memory>
#include <bitset>

struct Node;

using vshN = std::vector<std::shared_ptr<Node>>;

struct Node {
    char character;
    double probability;
    std::string code;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    
    Node(char ch, double pr) : character(ch), probability(pr), left(nullptr), right(nullptr) {}
};
 
struct CompareNodes {
    bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
        return lhs->probability > rhs->probability;
    }
};

void BuildTree(vshN& nodes) {
    if (nodes.empty()) return;
    
    std::priority_queue<std::shared_ptr<Node>, vshN, CompareNodes> min_heap;
    for (auto& node : nodes) min_heap.push(node);
    
    while (min_heap.size() > 1) {
        auto left = min_heap.top(); 
        min_heap.pop();
        
        auto right = min_heap.top(); 
        min_heap.pop();
        
        auto internal_node = std::make_shared<Node>('\0', left->probability + right->probability); 
        internal_node->left = left;
        internal_node->right = right;
        
        min_heap.push(internal_node);
    }
    
    auto root = min_heap.top();
    
    std::function<void(std::shared_ptr<Node>, std::string)> generate_codes = [&](std::shared_ptr<Node> node, std::string current_code) {
        if (!node) return;
        if (node->character != '\0') node->code = current_code;
        
        generate_codes(node->left, current_code + "0");
        generate_codes(node->right, current_code + "1");
    };
    
    generate_codes(root, "");
}

void compress(const std::string& file) {
    std::ifstream data_file(file, std::ios::ate);    
    if (!data_file.is_open()) throw std::runtime_error("Cannot find or open file");
    
    auto file_size = data_file.tellg();
    std::string data(file_size, '\0');
    
    data_file.seekg(0);
    data_file.read(&data[0], file_size);
    data_file.close();
    
    std::unordered_map<char, int> frequency;
    for (char ch : data) frequency[ch]++;
    
    vshN nodes;
    for (const auto& pair : frequency) 
        nodes.push_back(std::make_shared<Node>(pair.first, static_cast<double>(pair.second) / data.size()));
    
    BuildTree(nodes);
    
    std::unordered_map<char, std::string> codes;
    for (auto& node : nodes) {
        if (node->character != '\0') codes[node->character] = node->code;
    }
    
    // Prepare binary data using bitset for packing bits
    std::string bit_string;
    for (char ch : data) {
        bit_string += codes[ch];
    }
    
    // Pad the bit string to make its length a multiple of 8
    size_t padding = 8 - (bit_string.size() % 8);
    if (padding != 8) {
        bit_string.append(padding, '0');
    }
    
    // Write binary file
    std::ofstream compressed_file("compressed_text.bin", std::ios::binary);
    
    // 1. Write dictionary size (number of entries)
    size_t dict_size = codes.size();
    compressed_file.write(reinterpret_cast<const char*>(&dict_size), sizeof(dict_size));
    
    // 2. Write dictionary
    for (const auto& pair : codes) {
        compressed_file.write(&pair.first, sizeof(pair.first));
        uint8_t code_length = pair.second.size();
        compressed_file.write(reinterpret_cast<const char*>(&code_length), sizeof(code_length));
        
        // Convert code string to bits and write as bytes
        std::bitset<32> code_bits(pair.second); // Assuming codes are <= 32 bits
        uint32_t code_value = code_bits.to_ulong();
        compressed_file.write(reinterpret_cast<const char*>(&code_value), sizeof(code_value));
    }
    
    // 3. Write number of bits (original, without padding)
    size_t bit_count = bit_string.size() - padding;
    compressed_file.write(reinterpret_cast<const char*>(&bit_count), sizeof(bit_count));
    
    // 4. Write packed bits using bitset
    for (size_t i = 0; i < bit_string.size(); i += 8) {
        std::string byte_str = bit_string.substr(i, 8);
        std::bitset<8> byte_bits(byte_str);
        char byte = static_cast<char>(byte_bits.to_ulong());
        compressed_file.write(&byte, 1);
    }
    
    compressed_file.close();
    
    // Write human-readable dictionary for reference
    std::ofstream dictionary("dictionary.txt");
    for (const auto& pair : codes) {
        if (pair.first == ' ') dictionary << "' '" << " - " << pair.second << '\n';
        else dictionary << pair.first << " - " << pair.second << '\n';
    }
    dictionary.close();
}

void decompress(const std::string& bin) {
    std::ifstream binary_file(bin, std::ios::binary);
    if (!binary_file.is_open()) throw std::runtime_error("Cannot open binary file");
    
    // 1. Read dictionary size
    size_t dict_size;
    binary_file.read(reinterpret_cast<char*>(&dict_size), sizeof(dict_size));
    
    // 2. Read dictionary
    std::unordered_map<std::string, char> code_table;
    for (size_t i = 0; i < dict_size; ++i) {
        char character;
        binary_file.read(&character, sizeof(character));
        
        uint8_t code_length;
        binary_file.read(reinterpret_cast<char*>(&code_length), sizeof(code_length));
        
        uint32_t code_value;
        binary_file.read(reinterpret_cast<char*>(&code_value), sizeof(code_value));
        
        std::bitset<32> code_bits(code_value);
        std::string code = code_bits.to_string().substr(32 - code_length);
        code_table[code] = character;
    }
    
    // 3. Read number of bits
    size_t bit_count;
    binary_file.read(reinterpret_cast<char*>(&bit_count), sizeof(bit_count));
    
    // 4. Read packed bits
    std::string bit_string;
    size_t bytes_to_read = (bit_count + 7) / 8;
    for (size_t i = 0; i < bytes_to_read; ++i) {
        char byte;
        binary_file.read(&byte, 1);
        std::bitset<8> byte_bits(byte);
        bit_string += byte_bits.to_string();
    }
    // Remove padding bits
    bit_string.resize(bit_count);
    
    binary_file.close();
    
    // Decode the message
    std::string decoded_text;
    std::string current_code;
    for (char bit : bit_string) {
        current_code += bit;
        auto it = code_table.find(current_code);
        if (it != code_table.end()) {
            decoded_text += it->second;
            current_code.clear();
        }
    }
    
    std::ofstream decoded_file("decoded_text.txt");
    decoded_file << decoded_text;
    decoded_file.close();
}
