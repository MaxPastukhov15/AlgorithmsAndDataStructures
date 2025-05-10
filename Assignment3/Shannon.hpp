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

struct Node{
	char character;
	double probability;
	std::string code;
	Node* left;
	Node* right;
		
	Node(char ch, double pr) : character(ch), probability(pr), left(nullptr), right(nullptr) {}
};

// for encoding 
struct CompareNodes{
	bool operator()(Node* lhs, Node* rhs){
		return lhs->probability > rhs->probability;
	}
};

void BuildTree(std::vector<Node*>& nodes){
	std::priority_queue<Node*,std::vector<Node*>,CompareNodes> minHeap;
	for (auto node : nodes) minHeap.push(node);
	
	while(minHeap.size() > 1){
		Node* left = minHeap.top(); minHeap.pop();
		Node* right = MinHeap.top(); minHeap.pop();
		
		Node* internalNode = new Node('\0', left->probability + right->probability); 
		internalNode->left = left;
		internalNode->right = right;
		
		minHeap.push(internalNode);
	}
	
	Node* root = MinHeap.top();
	
	function<void(Node*, std::string)> generateCodes = [&](Node* node,std::string currentCode){
		if (!node) return;
		if (node->character != '\0') node->code = currentCode;
		
		generateCodes(node->left, currentCode + "0");
		generateCodes(node->right, currentCode + "1");
	};
        generateCodes(root, "");
}

std::string Compress(const std::string& data){
	std::unordered_map<char,int> frequency;
	for (char ch : data) frequency[ch]++;
	
	std::vector<Node*> nodes;
	for (const auto& pair : frequency) nodes.push_back(pair.first, static_cast<double>(pair.second) / data.size());
	
	buildTree(nodes);
	
	std::unordered_map<char,std::string> codes;
	for (auto node : nodes){
		if (node->character != '\0') codes[node->character] = node->code;
	}
	
	std::stringstream comprssedText;
	
	for(char ch : data) compressedText << codes[ch];
	
	std::ofstream compressedFile("compressed_text.txt");
	compressedFile << compressedText.str();
	compressedFile.close();
	
	std::ofstream Dictionary("dictionary.txt");
	for (const auto& pair : codes) Dictionary << pair.first << " - " << pair.second <<  '\n';	
	Dictionary.close();
	
	return compressedText.str();
}

// for decoding 

 
void decode(const std::string& dict, const std::string& bin){
     

}




