#ifndef DECODER_HPP
#define DECODER_HPP

#include <iostream>
#include <stdexcept>

void decoder(std::istream& input, std::ostream& output) {
    char buffer[5];
    int count = 0;

    while (input.get(buffer[count])) {
        //Checking end "~>"
        if (buffer[count] == '~' && input.peek() == '>') {
            input.get(); // Пропускаем '>'

            break;
        }

        // Checking corectness of symbols
        if (buffer[count] < 33 || buffer[count] > 117) {
            throw std::runtime_error("Invalid character in ASCII85 encoding");
        }

        count++;

        // If there are 5 symboles , we will decode them
        if (count == 5) {

            unsigned int value = 0;
            for (int i = 0; i < 5; ++i) {
                value = value * 85 + (buffer[i] - 33);
            }

            // Making a number
            char decoded[4] = {
                static_cast<char>((value >> 24) & 0xFF),
                static_cast<char>((value >> 16) & 0xFF),
                static_cast<char>((value >> 8) & 0xFF),
                static_cast<char>(value & 0xFF)
            };

            // Writeing decoded info
            output.write(decoded, 4);
            count = 0;
        }
    }

    // Working with last chunk (if exists)
    if (count > 0) {
        if (count < 2 || count > 4) {
            throw std::runtime_error("Incorrect final block size in ASCII85 encoding");
        }

        // Complete last block to 5 symbols
        for (int i = count; i < 5; ++i) {
            buffer[i] = 'u'; // 'u' соответствует 117 в ASCII

        }

        unsigned int value = 0;
        for (int i = 0; i < 5; ++i) {
            value = value * 85 + (buffer[i] - 33);
        }

        // Make from number back to symbols
        char decoded[4] = {
            static_cast<char>((value >> 24) & 0xFF),
            static_cast<char>((value >> 16) & 0xFF),
            static_cast<char>((value >> 8) & 0xFF),
            static_cast<char>(value & 0xFF)
        };

        // We write the decoded bytes, excluding padding
        output.write(decoded, count - 1);
    }
}


#endif
