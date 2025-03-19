#!/bin/bash


g++ -o ascii85 main.cpp encoder.hpp decoder.hpp -lgtest -lpthread
if [ $? -ne 0 ]; then
    echo "Ошибка компиляции"
    exit 1
fi

echo "Тестируем корректные данные..."
echo "Hello, World!" | ./ascii85 encode test_encoded.txt
encoded_cpp=$(cat test_encoded.txt)

python3 ascii.py <<< "Hello, World!" > test_python.txt
encoded_py=$(awk '{print $2}' test_python.txt)  

# Сравнение
if [ "$encoded_cpp" == "$encoded_py" ]; then
    echo "Кодирование совпадает!"
else
    echo "Кодирование различается!"
    exit 1
fi

echo "Тестируем декодирование..."
./ascii85 decode test_encoded.txt test_decoded_cpp.txt
python3 ascii.py <<< "Hello, World!" > test_python_decoded.txt
decoded_py=$(awk '{print $3}' test_python_decoded.txt)  

decoded_cpp=$(cat test_decoded_cpp.txt)

if [ "$decoded_cpp" == "$decoded_py" ]; then
    echo "Декодирование совпадает!"
else
    echo "Декодирование различается!"
    exit 1
fi

echo "Проверяем обработку некорректных данных..."
echo "!!!invalid_data!!!" > invalid_input.txt
./ascii85 decode invalid_input.txt invalid_output.txt
if [ $? -ne 0 ]; then
    echo "✅ C++ декодер корректно завершился с ошибкой!"
else
    echo "❌ C++ декодер пропустил некорректные данные!"
    exit 1
fi

echo "Тесты пройдены успешно!"
exit 0

