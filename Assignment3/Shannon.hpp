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

struct Node{
	char character;
	double probability;
	std::string code;
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
		
	Node(char ch, double pr) : character(ch), probability(pr), left(nullptr), right(nullptr) {}
};
 
struct CompareNodes{
	bool operator()(Node* lhs, Node* rhs){
		return lhs->probability > rhs->probability;
	}
};

void BuildTree(std::vector<std::unique_ptr<Node>>& nodes){
	std::priority_queue<Node*,std::vector<Node*>,CompareNodes> min_heap;
	for (auto& node : nodes) min_heap.push(node.get());
	
	while(min_heap.size() > 1){
		Node* left = min_heap.top(); min_heap.pop();
		Node* right = min_heap.top(); min_heap.pop();
		
		Node* internal_node = std::make_unique<Node>('\0', left->probability + right->probability); 
		internal_node->left.reset(left);
		internal_node->right.reset(right);
		
		min_heap.push(internalNode.get());
		min_heap.push_back(std::move(internal_node));
	}
	
	Node* root = min_heap.top();
	
	std::function<void(Node*, std::string)> generate_codes = [&](Node* node,std::string current_code){
		if (!node) return;
		if (node->character != '\0') node->code = current_code;
		
		generate_codes(node->left.get(), current_code + "0");
		generate_codes(node->right.get(), current_code + "1");
	};
        generate_codes(root, "");
}

//for encoding
void compress(const std::string& file){
	
	std::ifstream data_file(file, std::ios::ate);	
	if (!data_file.is_open()) throw std::runtime_error("Cannot find or open file");
	
	auto file_size = data_file.tellg();
	std::string data(file_size,'\0');
	
	data_file.seekg(0);
	data_file.read(&data[0], file_size);
	
	
	std::unordered_map<char,int> frequency;
	for (char ch : data) frequency[ch]++;
	
	data_file.close();
	
	std::vector<std::unique_ptr<Node>> nodes;
	for (const auto& pair : frequency) nodes.push_back(std::make_unique<Node>(pair.first, static_cast<double>(pair.second) / data.size()));
	
	BuildTree(nodes);
	
	std::unordered_map<char,std::string> codes;
	for (auto& node : nodes){
		if (node->character != '\0') codes[node->character] = node->code;
	}
	
	std::stringstream compressed_text;
	
	for(char ch : data) compressed_text << codes[ch];
	
	std::ofstream compressed_file("compressed_text.txt",std::ios::binary);
	compressed_file << compressed_text.str();
	compressed_file.close();
	
	std::ofstream dictionary("dictionary.txt");
	for (const auto& pair : codes) dictionary << pair.first << " - " << pair.second <<  '\n';	
	dictionary.close();
}

// for decoding 
void decompress(const std::string& dict, const std::string& bin){
     std::ifstream dictionary(dict);
     if (!dictionary.is_open()) throw std::runtime_error("Cannot open dictionary file");
     
     std::unordered_map<std::string,char> code_table;
     
     std::string line;
     while (std::getline(dictionary, line)){
           auto separator = line.find(" - ");
           if ((separator == 1) && (separator != std::string::npos)){
              std::string character = line[0];
              std::string code = line.substr(separator+3);
              code_table[code] = character;
           }
     }
     dictionary.close();
     
     std::ifstream binary_file(bin, std::ifstream::binary);
     if (!binary_file.is_open()) throw std::runtime_error("Cannot open binary file");
     
     auto length = binary_file.tellg();
     std::string data(length, '\0');
     binary_file.seekg(0);
     binary_file.read(&data[0],length);
     binary_file.close();
     
     std::string decoded_text = "";
     std::string current_code = "";
     
     for (char bit : data){
     	current_code += bit;
     	if(code_table.find(current_code) != code_table.end()) {
     		decoded_text += code_table[current_code];
     		current_code = "";
     	}
     	
     }
     
     std::ofstream decoded_file("decoded_text.txt");
     decoded_file << decoded_text;
     decoded_file.close();
}




