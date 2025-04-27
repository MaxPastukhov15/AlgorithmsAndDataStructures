#include <gtest/gtest.h>
#include "Shannon.hpp"
#include <fstream>
#include <filesystem>

class ShannonCodecTest : public ::testing::Test {
protected:
    ShannonCodec codec;

    void SetUp() override {
    // Create some test files
    std::ofstream simple("simple.txt");
    simple << "ABRACADABRA";
    simple.close();

    std::ofstream binary("binary.bin", std::ios::binary);
    // Create a file with repeating patterns
    for (int i = 0; i < 256; ++i) {
        binary.put(static_cast<char>(i % 10)); // Only use byte values 0-9
    }
    binary.close();

    std::ofstream empty("empty.bin", std::ios::binary);
    empty.close();

    std::ofstream corrupt("corrupt.dict", std::ios::binary);
    corrupt << "corrupted content";
    corrupt.close();
    }


    void TearDown() override {
        std::filesystem::remove("simple.txt");
        std::filesystem::remove("binary.bin");
        std::filesystem::remove("empty.bin");
        std::filesystem::remove("corrupt.dict");
        std::filesystem::remove("compressed.bin");
        std::filesystem::remove("decompressed.bin");
        std::filesystem::remove("dictionary.dict");
    }
};

TEST_F(ShannonCodecTest, DictionaryCreation) {
    codec.buildDictionary("simple.txt");
    EXPECT_GT(codec.getEncodingDict().size(), 0);
    EXPECT_GT(codec.getDecodingDict().size(), 0);
}

TEST_F(ShannonCodecTest, DictionarySaveLoad) {
    codec.buildDictionary("simple.txt");
    codec.saveDictionary("dictionary.dict");

    ShannonCodec loadedCodec;
    loadedCodec.loadDictionary("dictionary.dict");

    EXPECT_EQ(codec.getEncodingDict().size(), loadedCodec.getEncodingDict().size());
    EXPECT_EQ(codec.getDecodingDict().size(), loadedCodec.getDecodingDict().size());
}

TEST_F(ShannonCodecTest, EmptyFileEncoding) {
    codec.buildDictionary("simple.txt");
    EXPECT_NO_THROW(codec.encode("empty.bin", "compressed.bin"));
}

TEST_F(ShannonCodecTest, SimpleTextEncodingDecoding) {
    codec.buildDictionary("simple.txt");
    codec.saveDictionary("dictionary.dict");
    codec.encode("simple.txt", "compressed.bin");

    ShannonCodec decoder;
    decoder.loadDictionary("dictionary.dict");
    decoder.decode("compressed.bin", "decompressed.bin");

    std::ifstream original("simple.txt", std::ios::binary);
    std::ifstream decompressed("decompressed.bin", std::ios::binary);

    std::string origContent((std::istreambuf_iterator<char>(original)), {});
    std::string decompContent((std::istreambuf_iterator<char>(decompressed)), {});

    EXPECT_EQ(origContent, decompContent);
}

TEST_F(ShannonCodecTest, BinaryFileEncodingDecoding) {
    codec.buildDictionary("binary.bin");
    codec.saveDictionary("dictionary.dict");
    codec.encode("binary.bin", "compressed.bin");

    ShannonCodec decoder;
    decoder.loadDictionary("dictionary.dict");
    decoder.decode("compressed.bin", "decompressed.bin");

    std::ifstream original("binary.bin", std::ios::binary);
    std::ifstream decompressed("decompressed.bin", std::ios::binary);

    std::string origContent((std::istreambuf_iterator<char>(original)), {});
    std::string decompContent((std::istreambuf_iterator<char>(decompressed)), {});

    EXPECT_EQ(origContent, decompContent);
}

TEST_F(ShannonCodecTest, CompressionRatio) {
    codec.buildDictionary("binary.bin");
    codec.encode("binary.bin", "compressed.bin");

    std::ifstream original("binary.bin", std::ios::binary | std::ios::ate);
    std::ifstream compressed("compressed.bin", std::ios::binary | std::ios::ate);

    auto origSize = original.tellg();
    auto compSize = compressed.tellg();

    EXPECT_LT(compSize, origSize);
}

TEST_F(ShannonCodecTest, InvalidDictionaryFile) {
    ShannonCodec decoder;
    EXPECT_THROW(decoder.loadDictionary("nonexistent.dict"), std::runtime_error);
}

TEST_F(ShannonCodecTest, InvalidInputFile) {
    codec.buildDictionary("simple.txt");
    codec.saveDictionary("dictionary.dict");

    EXPECT_THROW(codec.encode("nonexistent.txt", "compressed.bin"), std::runtime_error);

    ShannonCodec decoder;
    decoder.loadDictionary("dictionary.dict");
    EXPECT_THROW(decoder.decode("nonexistent.bin", "decompressed.bin"), std::runtime_error);
}

TEST_F(ShannonCodecTest, FrequencyAnalysis) {
    codec.buildDictionary("simple.txt");
    EXPECT_GT(codec.getEncodingDict().size(), 0);
}

