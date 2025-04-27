#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdint>

class ShannonCodec {
private:
    std::unordered_map<char, std::string> encodingDict;
    std::unordered_map<std::string, char> decodingDict;

    
    void shannonFano(int start, int end, const std::vector<double>& prob, std::vector<std::string>& codes);

public:
    void buildDictionary(const std::string& inputFilename);
    void saveDictionary(const std::string& dictFilename) const;
    void loadDictionary(const std::string& dictFilename);

    void encode(const std::string& inputFilename, const std::string& outputFilename) const;
    void decode(const std::string& inputFilename, const std::string& outputFilename) const;

    const std::unordered_map<char, std::string>& getEncodingDict() const { return encodingDict; }
    const std::unordered_map<std::string, char>& getDecodingDict() const { return decodingDict; }
};


