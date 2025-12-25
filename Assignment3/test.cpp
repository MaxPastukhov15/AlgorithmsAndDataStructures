#include <gtest/gtest.h>
#include "Shannon.hpp"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string read_file(const std::string& file_name){
	std::ifstream file(file_name);
	
	std::stringstream buffer;
	std::string line;
	buffer << file.rdbuf();
	
	return buffer.str();
}


TEST(ShannonTest, SimpleTest){
	std::ofstream out("input.txt");
	out << "hello";
	out.close();
	
	compress("input.txt");
	

	
	decompress("dictionary.txt", "compressed_text.bin");
	
	std::string decoded = read_file("decoded_text.txt");
	
	
	ASSERT_EQ(decoded, "hello");
	
	fs::remove("input.txt");
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.bin");
	fs::remove("decoded_text.txt");
}

TEST(ShannonTest, TextWithSpaces){
	std::ofstream out("input.txt");
	out << "hello world";
	out.close();
	
	compress("input.txt");
	decompress("dictionary.txt", "compressed_text.bin");
	
	
	std::string decoded = read_file("decoded_text.txt");
	
	ASSERT_EQ(decoded, "hello world");
	
	fs::remove("input.txt");
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.bin");
	fs::remove("decoded_text.txt");
}

TEST(ShannonTest, EmptyFile){
	std::ofstream out("input.txt");
	out << "";
	out.close();
	
	compress("input.txt");
	decompress("dictionary.txt", "compressed_text.bin");
	
	std::string decoded = read_file("decoded_text.txt");
	
	ASSERT_EQ(decoded,"");
	
	fs::remove("input.txt");
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.bin");
	fs::remove("decoded_text.txt");
}

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}









