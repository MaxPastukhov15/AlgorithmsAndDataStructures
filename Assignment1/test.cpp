#include "encoder.hpp"
#include "decoder.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(ASCII_85Test, EncodeEmptyInput) {
    std::istringstream input("");
    std::ostringstream output;
    encoder(input, output);
    EXPECT_EQ(output.str(), "~>");
}

TEST(ASCII_85Test, EncodeSingleByte) {
    std::istringstream input("A");
    std::ostringstream output;
    encoder(input, output);
    EXPECT_EQ(output.str().substr(0, 2), "5l");  // Ignore "~>" for decoding test
}

TEST(ASCII_85Test, DecodeSingleByte) {
    std::istringstream input("5l~>");
    std::ostringstream output;
    decoder(input, output);
    EXPECT_EQ(output.str(), "A");
}

TEST(ASCII85_Test, EncodeDecodeRoundTrip) {
    std::string original = "Hello, World!";
    std::istringstream input(original);
    std::ostringstream encoded_output;
    encoder(input, encoded_output);

    std::istringstream encoded_input(encoded_output.str());
    std::ostringstream decoded_output;
    decoder(encoded_input, decoded_output);

    EXPECT_EQ(decoded_output.str(), original);
}

TEST(ASCII_85Test, DecodeInvalidCharacter) {
    std::istringstream input("5l~!");  // "~!" is invalid, should throw an error
    std::ostringstream output;
    EXPECT_THROW(decoder(input, output), std::runtime_error);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

