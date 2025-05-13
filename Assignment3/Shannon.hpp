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

struct Node;

using vshN = std::vector<std::shared_ptr<Node>>;

struct Node{
	char character;
	double probability;
	std::string code;
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
		
	Node(char ch, double pr) : character(ch), probability(pr), left(nullptr), right(nullptr) {}
};
 
struct CompareNodes{
	bool operator()(const std::shared_ptr<Node>& lhs,const std::shared_ptr<Node>& rhs) const{
		return lhs->probability > rhs->probability;
	}
};
//data structure generator for dictionary 
void BuildTree(vshN& nodes){
	if (nodes.empty()) return;
	
	std::priority_queue<std::shared_ptr<Node>,vshN,CompareNodes> min_heap;
	for (auto& node : nodes) min_heap.push(node);
	
	while(min_heap.size() > 1){
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
	
	std::function<void(std::shared_ptr<Node>, std::string)> generate_codes = [&](std::shared_ptr<Node> node,std::string current_code){
		if (!node) return;
		if (node->character != '\0') node->code = current_code;
		
		generate_codes(node->left, current_code + "0");
		generate_codes(node->right, current_code + "1");
	};
	
	generate_codes(root, "");
}

//encoder
void compress(const std::string& file){
	
	std::ifstream data_file(file, std::ios::ate);	
	if (!data_file.is_open()) throw std::runtime_error("Cannot find or open file");
	
	auto file_size = data_file.tellg();
	std::string data(file_size,'\0');
	
	data_file.seekg(0);
	data_file.read(&data[0], file_size);
	data_file.close();
	
	std::unordered_map<char,int> frequency;
	for (char ch : data) frequency[ch]++;
	
	
	
	vshN nodes;
	for (const auto& pair : frequency) nodes.push_back(std::make_shared<Node>(pair.first, static_cast<double>(pair.second) / data.size()));
	
	BuildTree(nodes);
	
	std::unordered_map<char,std::string> codes;
	for (auto& node : nodes){
		if (node->character != '\0') codes[node->character] = node->code;
	}
	
	std::stringstream compressed_text;
	
	for(char ch : data) compressed_text << codes[ch];
	
	std::ofstream compressed_file("compressed_text.bin",std::ios::binary);
	compressed_file << compressed_text.str();
	compressed_file.close();
	
	std::ofstream dictionary("dictionary.txt");
	for (const auto& pair : codes){
		if (pair.first == ' ') dictionary << "' '" << " - " << pair.second << '\n';
		else dictionary << pair.first << " - " << pair.second << '\n';
	}
		
	dictionary.close();
}

//decoder 
void decompress(const std::string& dict, const std::string& bin){
     std::ifstream dictionary(dict);
     if (!dictionary.is_open()) throw std::runtime_error("Cannot open dictionary file");
     
     std::unordered_map<std::string,char> code_table;
     
     std::string line;
     while (std::getline(dictionary, line)){
           auto sep = line.find(" - ");
           if (sep != std::string::npos){
           	std::string character_str = line.substr(0,sep);
           	char character = (character_str == "' '" ? ' ' : character_str[0]);
           	std::string code = line.substr(sep + 3);
           	code_table[code] = character;
           }
     }
     dictionary.close();
     
     std::ifstream binary_file(bin, std::ifstream::binary);
     if (!binary_file.is_open()) throw std::runtime_error("Cannot open binary file");
     
     auto length = binary_file.tellg();
     std::string data(length, '\0');
     binary_file.seekg(0);
     binary_file.read(&data[0], length);
     binary_file.close();
     	
     std::string decoded_text = "";
     std::string current_code = "";
     
     for (char bit : data){
     	current_code += bit;
     	if (code_table.find(current_code) != code_table.end()){
     		decoded_text += code_table[current_code];
     		current_code = "";
     	}
     }     
     	
     std::ofstream decoded_file("decoded_text.txt");
     decoded_file << decoded_text;
     decoded_file.close();
}




