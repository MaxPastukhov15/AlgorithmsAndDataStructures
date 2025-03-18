#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <iostream>
#include <vector>

void encoder(std::istream& input, std::ostream& output) {
    std::vector<char> buffer(4);
    while (!input.bad()) {
        input.read(buffer.data(), 4);
        std::streamsize bytes_read = input.gcount();

        if (bytes_read == 0) break;

        // Pad with zeros if necessary
        int padding = 0;
        if (bytes_read < 4) {
            padding = 4 - bytes_read;
            for (int i = bytes_read; i < 4; ++i) buffer[i] = 0;
        }

        unsigned int value = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];

        char encode[5];
        for (int i = 4; i >= 0; --i) {
            encode[i] = value % 85 + 33;
            value /= 85;
        }

        // Write only the necessary characters (5 for full block, fewer for partial)
        output.write(encode, 5 - padding);
    }

    output << "~>";
}
#endif
