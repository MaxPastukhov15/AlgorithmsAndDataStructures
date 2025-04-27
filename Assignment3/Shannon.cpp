#include "Shannon.hpp"
#include <queue>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <cmath> 
#include <stack>

void ShannonCodec::buildDictionary(const std::string& inputFilename) {
    std::ifstream file(inputFilename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open input file for building dictionary.");
    }

    std::unordered_map<char, int> freq;
    char ch;
    while (file.get(ch)) {
        freq[ch]++;
    }
    file.close();

    encodingDict.clear();
    decodingDict.clear();

    // Convert to vector and sort by frequency (descending)
    std::vector<std::pair<char, int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    // Calculate total frequency
    int total = 0;
    for (const auto& pair : sorted) {
        total += pair.second;
    }

    // Build codes using Shannon-Fano algorithm
    std::vector<double> prob;
    for (const auto& pair : sorted) {
        prob.push_back(static_cast<double>(pair.second) / total);
    }

    std::vector<std::string> codes(sorted.size());
    shannonFano(0, sorted.size() - 1, prob, codes);

    // Assign codes to symbols
    for (size_t i = 0; i < sorted.size(); ++i) {
        encodingDict[sorted[i].first] = codes[i];
        decodingDict[codes[i]] = sorted[i].first;
    }
}

void ShannonCodec::shannonFano(int start, int end,const std::vector<double>& prob,std::vector<std::string>& codes) {
    struct Frame {
        int start;
        int end;
        std::string prefix;
    };

    std::stack<Frame> stack;
    stack.push({start, end, ""});

    while (!stack.empty()) {
        Frame frame = stack.top();
        stack.pop();

        int currentStart = frame.start;
        int currentEnd = frame.end;
        std::string prefix = frame.prefix;

        if (currentStart == currentEnd) {
            codes[currentStart] = prefix;
            continue;
        }

        if (currentStart == currentEnd - 1) {
            codes[currentStart] = prefix + "0";
            codes[currentEnd] = prefix + "1";
            continue;
        }

        // Find split point
        double total = 0;
        for (int i = currentStart; i <= currentEnd; ++i) {
            total += prob[i];
        }

        double half = total / 2;
        double sum = 0;
        int split = currentStart;
        while (split <= currentEnd && sum < half) {
            sum += prob[split];
            split++;
        }

        // Push right group onto stack
        stack.push({split, currentEnd, prefix + "1"});

        // Push left group onto stack
        stack.push({currentStart, split - 1, prefix + "0"});
    }
}

void ShannonCodec::saveDictionary(const std::string& dictFilename) const {
    std::ofstream out(dictFilename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open dictionary file for writing.");
    }

    uint32_t size = encodingDict.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (auto& [ch, code] : encodingDict) {
        out.put(ch);
        uint32_t len = code.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(code.c_str(), len);
    }
}

void ShannonCodec::loadDictionary(const std::string& dictFilename) {
    std::ifstream in(dictFilename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open dictionary file for reading.");
    }

    encodingDict.clear();
    decodingDict.clear();

    uint32_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));

    for (uint32_t i = 0; i < size; ++i) {
        char ch = in.get();
        if (!in) {
            throw std::runtime_error("Failed to read character from dictionary file.");
        }

        uint32_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) {
            throw std::runtime_error("Failed to read code length from dictionary file.");
        }

        std::string code(len, '\0');
        in.read(&code[0], len);
        if (!in) {
            throw std::runtime_error("Failed to read code from dictionary file.");
        }

        encodingDict[ch] = code;
        decodingDict[code] = ch;
    }
}


void ShannonCodec::encode(const std::string& inputFilename, const std::string& outputFilename) const {
    std::ifstream in(inputFilename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open input file for encoding.");
    }
    std::ofstream out(outputFilename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open output file for encoding.");
    }

    std::string bitBuffer;
    char ch;
    while (in.get(ch)) {
        auto it = encodingDict.find(ch);
        if (it == encodingDict.end()) {
            throw std::runtime_error("Character not found in encoding dictionary.");
        }
        bitBuffer += it->second;

        // Write complete bytes as they're formed
        while (bitBuffer.size() >= 8) {
            std::bitset<8> bits(bitBuffer.substr(0, 8));
            out.put(static_cast<unsigned char>(bits.to_ulong()));
            bitBuffer = bitBuffer.substr(8);
        }
    }

    // Write remaining bits (pad with zeros if needed)
    if (!bitBuffer.empty()) {
        while (bitBuffer.size() < 8) {
            bitBuffer += "0";
        }
        std::bitset<8> bits(bitBuffer);
        out.put(static_cast<unsigned char>(bits.to_ulong()));
    }

    in.close();
    out.close();
}
void ShannonCodec::decode(const std::string& inputFilename, const std::string& outputFilename) const {
    std::ifstream in(inputFilename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open input file for decoding.");
    }
    std::ofstream out(outputFilename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open output file for decoding.");
    }

    std::string bitBuffer;
    char byte;
    while (in.get(byte)) {
        std::bitset<8> bits(static_cast<unsigned char>(byte));
        bitBuffer += bits.to_string();
    }

    std::string currentCode;
    for (char bit : bitBuffer) {
        currentCode += bit;
        auto it = decodingDict.find(currentCode);
        if (it != decodingDict.end()) {
            out.put(it->second);
            currentCode.clear();
        }
    }

    in.close();
    out.close();
}

