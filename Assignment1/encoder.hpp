#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <iostream>

void encoder(std::istream& input, std::ostream& output) {
    char buffer[4];
    while (input.read(buffer, 4) || input.gcount() > 0) {
        std::streamsize bytes_read = input.gcount();
        int padding = 4 - bytes_read;

        //If the data is less than 4 bytes, add zeros.
        for (int i = bytes_read; i < 4; ++i) {
            buffer[i] = 0;
        }

       unsigned int value = (static_cast<unsigned char>(buffer[0]) << 24 |
                      (static_cast<unsigned char>(buffer[1]) << 16) |
                      (static_cast<unsigned char>(buffer[2]) << 8) |
                      static_cast<unsigned char>(buffer[3]));



        // Encoding a 32-bit number into 5 ASCII85 characters
        char encode[5];
        for (int i = 4; i >= 0; --i) {
            encode[i] = value % 85 + 33;
            value /= 85;
        }

        // We write down the encoded characters, excluding padding

        output.write(encode, 5 - padding);
    }

    // Writing down the end marker
    output << "~>";
}

#endif

