#include "encoder.hpp"
#include "decoder.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include "encoder.hpp"
#include "decoder.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>
#include <ctime>


std::string generate_random_string(size_t length) {
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789!@#$%^&*()-_=+[]{}|;:'\",.<>?/~` ";
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[rand() % (sizeof(charset) - 1)];
    }
    return result;
}

TEST(ASCII85_TestL, RandomEncodeDecode) {
    srand(time(nullptr));
    for (int i = 0; i < 10; ++i) {  
        std::string original = generate_random_string(50 + rand() % 50);
        std::istringstream input(original);
        std::ostringstream encoded_output;
        encoder(input, encoded_output);

        std::istringstream encoded_input(encoded_output.str());
        std::ostringstream decoded_output;
        decoder(encoded_input, decoded_output);

        EXPECT_EQ(decoded_output.str(), original);
    }
}

TEST(ASCII85_TestL, InvalidDecoding) {
    srand(time(nullptr));
    for (int i = 0; i < 5; ++i) {  // 5 итераций
        std::string invalid_data = generate_random_string(20 + rand() % 30);
        std::istringstream input(invalid_data);
        std::ostringstream output;

        EXPECT_THROW(decoder(input, output), std::runtime_error);
    }
}

TEST(ASCII85_TestL, DecodeInvalidCharacter) {
    std::istringstream input("hello$~>");
    std::ostringstream output;
    EXPECT_THROW(decoder(input, output), std::runtime_error);
}

TEST(ASCII85_TestL, TruncatedInput) {
    std::istringstream input("9ar~");  
    std::ostringstream output;
    EXPECT_THROW(decoder(input, output), std::runtime_error);
}


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
    EXPECT_EQ(output.str().substr(0, 2), "5l");
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
    std::istringstream input("5l~!");  
    std::ostringstream output;
    EXPECT_THROW(decoder(input, output), std::runtime_error);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

