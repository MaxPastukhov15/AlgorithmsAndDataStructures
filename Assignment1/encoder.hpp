#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <iostream>
#include <vector>

void encoder(std::istream& input, std::ostream& output) {
    std::vector<char> buffer(4);

    while (input.read(buffer.data(), 4) || input.gcount() > 0) {
        std::streamsize bytes_read = input.gcount();
        int padding = 4 - bytes_read;

        for (int i = bytes_read; i < 4; ++i) buffer[i] = 0;

        unsigned int value = (static_cast<unsigned char>(buffer[0]) << 24) |
                             (static_cast<unsigned char>(buffer[1]) << 16) |
                             (static_cast<unsigned char>(buffer[2]) << 8) |
                             static_cast<unsigned char>(buffer[3]);

        char encode[5];
        for (int i = 4; i >= 0; --i) {
            encode[i] = value % 85 + 33;
            value /= 85;
        }

        output.write(encode, 5 - padding);
    }

    output << "~>";
}

#endif

