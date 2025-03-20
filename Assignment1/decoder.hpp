#ifndef DECODER_HPP
#define DECODER_HPP

#include <iostream>
#include <stdexcept>

void decoder(std::istream& input, std::ostream& output) {
    char buffer[5];
    int count = 0;

    while (input.get(buffer[count])) {
        // Проверяем маркер конца "~>"
        if (buffer[count] == '~' && input.peek() == '>') {
            input.get(); // Пропускаем '>'
            break;
        }

        // Проверяем корректность символа
        if (buffer[count] < 33 || buffer[count] > 117) {
            throw std::runtime_error("Invalid character in ASCII85 encoding");
        }

        count++;

        // Если накопилось 5 символов, декодируем их
        if (count == 5) {
            unsigned int value = 0;
            for (int i = 0; i < 5; ++i) {
                value = value * 85 + (buffer[i] - 33);
            }

            // Преобразуем 32-битное число в 4 байта
            char decoded[4] = {
                static_cast<char>((value >> 24) & 0xFF),
                static_cast<char>((value >> 16) & 0xFF),
                static_cast<char>((value >> 8) & 0xFF),
                static_cast<char>(value & 0xFF)
            };

            // Записываем декодированные байты
            output.write(decoded, 4);
            count = 0;
        }
    }

    // Обработка последнего блока (если он есть)
    if (count > 0) {
        if (count < 2 || count > 4) {
            throw std::runtime_error("Incorrect final block size in ASCII85 encoding");
        }

        // Дополняем последний блок до 5 символов
        for (int i = count; i < 5; ++i) {
            buffer[i] = 'u'; // 'u' соответствует 117 в ASCII
        }

        unsigned int value = 0;
        for (int i = 0; i < 5; ++i) {
            value = value * 85 + (buffer[i] - 33);
        }

        // Преобразуем 32-битное число в байты
        char decoded[4] = {
            static_cast<char>((value >> 24) & 0xFF),
            static_cast<char>((value >> 16) & 0xFF),
            static_cast<char>((value >> 8) & 0xFF),
            static_cast<char>(value & 0xFF)
        };

        // Записываем декодированные байты, исключая padding
        output.write(decoded, count - 1);
    }
}

#endif
