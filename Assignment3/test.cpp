#include <gtest/gtest.h>
#include "Shannon.hpp"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string read_file(const std::string& file_name){
	std::ifstream file(file_name);
	if (1file.is_open()) throw std::runtime_error("Cannot open file");
	
	std::string content;
	std::string line;
	while(std::getline(file,line)){
		content += line;
		if (!file.eof()) content += '\n';
	}
	return content;
}


TEST(ShannonTest, CompressionDecompressionTest){
	std::ofstream test("test.txt");
	test << "hello world";
	test.close();
	
	ASSERT_NO_THROW(compress("test.txt"));
	
	ASSERT_TRUE(fs::exists("dictionary.txt"));
	ASSERT_TRUE(fs::exists("compressed_text.txt"));
	
	ASSERT_NO_THROW(decompress("dictionary.txt", "compressed_text.txt"));
	
	ASSERT_TRUE(fs::exists("decoded_text.txt"));
	
	std::string decoded_content = read_file("decoded_text.txt");
	ASSERT_EQ(test_content, decoded_content);
	
	fs::remove("test.txt");
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.txt");
	fs::remove("decoded_text.txt");
}

TEST(ShannonTest, EmptyFile){
	std::ofstream("empty.txt").close();
	
	ASSERT_NO_THROW(compress("empty.txt"));
	ASSERT_NO_THROW(decompress("dictionary.txt", "compressed_text"));
	
	std::ifstream decoded("decoded_text.txt");
	std::string line;
	ASSERT_FALSE(std::getline(decoded,line));
	
	fs::remove("empty.txt");
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.txt");
	fs::remove("decoded_text.txt");
}

TEST(ShannonTest, FileNotFound){
	ASSERT_THROW(compress("nonexistent.txt"), std::runtime_error);
	ASSERT_THROW(decompress("nonexistent_dict.txt","nonexistent_bin.txt"), std::runtime_error);
}

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}









