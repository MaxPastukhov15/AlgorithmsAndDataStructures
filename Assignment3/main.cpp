#include "Shannon.hpp"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem; 

int main(){
    
   std::string name;
   std::cout << "Write name of file\n";
   std::cin >> name;
   
   compress(name);
   
   decompress("dictionary.txt", "compressed_text.txt");
   
   char ans;
   std::cout << "delete file? y(if you want to delete)\n";
   std::cin >> ans;
   if (ans == "y"){
   	fs::remove(name);
	fs::remove("dictionary.txt");
	fs::remove("compressed_text.txt");
	fs::remove("decoded_text.txt");
   }
   else{
	return;      
   }
   
   return 0;
}

