#!/bin/bash

# Компиляция программы
g++ -o ascii85 main.cpp encoder.hpp decoder.hpp -lgtest -lpthread
if [ $? -ne 0 ]; then
    echo "Ошибка компиляции"
    exit 1
fi

# Создание тестового входного файла
echo "Hello, World!" > test_input.txt

# Тестирование кодирования
echo "Тестируем корректные данные..."
encoded_cpp=$(./ascii85 -e < test_input.txt)

# Кодирование с помощью Python для сравнения
encoded_py=$(python3 ascii.py <<< "Hello, World!")

# Удаляем маркер конца для сравнения
encoded_cpp_clean=$(echo "$encoded_cpp" | sed 's/~>$//')
encoded_py_clean=$(echo "$encoded_py" | sed 's/~>$//')

# Сравнение закодированных данных
if [ "$encoded_cpp_clean" == "$encoded_py_clean" ]; then
    echo "Кодирование совпадает!"
else
    echo "Кодирование различается!"
    echo "C++: $encoded_cpp"
    echo "Python: $encoded_py"
    exit 1
fi

# Тестирование декодирования
echo "Тестируем декодирование..."
decoded_cpp=$(./ascii85 -d <<< "$encoded_cpp")

# Декодирование с помощью Python для сравнения
decoded_py=$(python3 ascii.py <<< "Hello, World!" | awk '{print $3}')

# Сравнение декодированных данных
if [ "$decoded_cpp" == "$decoded_py" ]; then
    echo "Декодирование совпадает!"
else
    echo "Декодирование различается!"
    echo "C++: $decoded_cpp"
    echo "Python: $decoded_py"
    exit 1
fi

# Тестирование обработки некорректных данных
echo "Проверяем обработку некорректных данных..."
echo "!!!invalid_data!!!" > invalid_input.txt
./ascii85 -d < invalid_input.txt > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "✅ C++ декодер корректно завершился с ошибкой!"
else
    echo "❌ C++ декодер пропустил некорректные данные!"
    exit 1
fi

echo "Тесты пройдены успешно!"
exit 0
