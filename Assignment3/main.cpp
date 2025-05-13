#include "Shannon.hpp"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem; 

int main(){
    
   std::string name;
   std::cout << "Write name of file\n";
   std::cin >> name;
   
   try{
   	compress("text.txt");
   
   	decompress("dictionary.txt", "compressed_text.txt");
   	
   	char ans;
   	std::cout << "delete file? y/n\n";
   	std::cin >> ans;
   	if (ans == 'y'){
   		fs::remove(name);
		fs::remove("dictionary.txt");
		fs::remove("compressed_text.txt");
		fs::remove("decoded_text.txt");
   	}
  } catch (const std::exception& e){
   	std::cerr << "Error: " << e.what() << '\n';
   	return 1;
  }
   return 0;
}

