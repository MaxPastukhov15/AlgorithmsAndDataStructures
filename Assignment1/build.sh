#!/bin/bash

set -e

# Компиляция программы
echo "Компиляция C++ программы..."
g++ -o ascii85 main.cpp encoder.hpp decoder.hpp || {
    echo "Ошибка компиляции"
    exit 1
}

# Создание тестового файла
echo "Создание тестовых данных..."
echo "Hello World" > test_input.txt
original_content=$(cat test_input.txt)

# Тест 1: C++ encode -> Python decode
echo -e "\nТест 1: C++ encode -> Python decode"
encoded_cpp=$(./ascii85 -e < test_input.txt)
echo "Закодированные данные (C++): ${encoded_cpp}"

# Удаляем терминатор ~> для Python декодера
encoded_for_py=${encoded_cpp//~>/}

decoded_py=$(python3 -c "
import base64
try:
    decoded = base64.a85decode(b'$encoded_for_py', adobe=False)
    print(decoded.decode('utf-8'))
except Exception as e:
    print('DECODE_ERROR:', str(e))
")

if [ "$decoded_py" == "$original_content" ]; then
    echo "Успех: C++ encode -> Python decode"
else
    echo "Ошибка: C++ encode -> Python decode"
    echo "Ожидалось: '$original_content'"
    echo "Получено:  '$decoded_py'"
    exit 1
fi

# Тест 2: Python encode -> C++ decode
echo -e "\nТест 2: Python encode -> C++ decode"
encoded_py=$(python3 -c "
import base64
import sys
data = sys.stdin.read().encode('utf-8')
encoded = base64.a85encode(data, adobe=False).decode('utf-8')
print(encoded + '~>')
" < test_input.txt)

echo "Закодированные данные (Python): ${encoded_py}"

decoded_cpp=$(./ascii85 -d <<< "$encoded_py" 2>&1 || echo "DECODE_ERROR: $?")

if [ "$decoded_cpp" == "$original_content" ]; then
    echo "Успех: Python encode -> C++ decode"
else
    echo "Ошибка: Python encode -> C++ decode"
    echo "Ожидалось: '$original_content'"
    echo "Получено:  '$decoded_cpp'"
    exit 1
fi

echo -e "\nВсе тесты совместимости пройдены успешно!"
exit 0
=======


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


