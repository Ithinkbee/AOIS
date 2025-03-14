#include <iostream>
#include <string>
#include <algorithm>
#include <clocale> 
#include <iomanip> 
#include <cmath>  

static int MAX_BINARY_LENGTH = 8;
static int MANTISSA_BITS = 23;
static int EXPONENT_BIAS = 127;
static int NEGATIVE_SIGN_BIT = 1;
static std::string FORMAT_8_BIT_BINARY = "00000001";

// Функция для перевода числа в двоичный прямой код
std::string toDirectCode(int number, int bits = MAX_BINARY_LENGTH) {
    std::string binaryRepresentation;
    bool isNegative = number < 0;
    number = abs(number); // Работаем с положительным числом

    // Переводим число в двоичный вид
    for (int bitPosition = bits - 1; bitPosition >= 0; --bitPosition) {
        binaryRepresentation += ((number >> bitPosition) & 1) ? '1' : '0';
    }

    // Если число отрицательное, устанавливаем знаковый бит
    if (isNegative) {
        binaryRepresentation[0] = NEGATIVE_SIGN_BIT;
    }

    return binaryRepresentation;
}

// Функция для перевода числа в двоичный обратный код
std::string toInverseCode(int number, int bits = MAX_BINARY_LENGTH) {
    std::string directCode = toDirectCode(number, bits);
    if (number >= 0) {
        return directCode;
    }
    for (size_t bitIndex = 1; bitIndex < directCode.length(); ++bitIndex) {
        directCode[bitIndex] = (directCode[bitIndex] == '0') ? '1' : '0';
    }
    return directCode;
}

// Функция для перевода числа в двоичный дополнительный код
std::string toComplementCode(int number, int bits = MAX_BINARY_LENGTH) {
    std::string inverseCode = toInverseCode(number, bits);
    if (number >= 0) {
        return inverseCode;
    }
    // Добавляем 1 к обратному коду
    bool carry = true;
    for (int bitIndex = inverseCode.length() - 1; bitIndex >= 0; --bitIndex) {
        if (carry) {
            if (inverseCode[bitIndex] == '0') {
                inverseCode[bitIndex] = '1';
                carry = false;
            }
            else {
                inverseCode[bitIndex] = '0';
            }
        }
    }
    return inverseCode;
}

// Функция для сложения двух двоичных чисел
std::string binaryAddition(const std::string& binaryNumber1, const std::string& binaryNumber2) {
    std::string result = "";
    int carry = 0;
    int index1 = binaryNumber1.length() - 1;
    int index2 = binaryNumber2.length() - 1;

    while (index1 >= 0 || index2 >= 0 || carry) {
        int sum = carry;
        if (index1 >= 0) sum += binaryNumber1[index1--] - '0';
        if (index2 >= 0) sum += binaryNumber2[index2--] - '0';
        carry = sum / 2;
        result = char(sum % 2 + '0') + result;
    }

    return result;
}

// Функция для перевода двоичного числа в десятичное
int binaryToDecimal(const std::string& binaryNumber) {
    int decimalNumber = 0;
    int power = 1;
    for (int bitIndex = binaryNumber.length() - 1; bitIndex >= 0; --bitIndex) {
        if (binaryNumber[bitIndex] == '1') {
            decimalNumber += power;
        }
        power *= 2;
    }
    // Если число отрицательное (знаковый бит установлен)
    if (binaryNumber[0] == NEGATIVE_SIGN_BIT) {
        decimalNumber -= power;
    }
    return decimalNumber;
}

// Функция для вычитания
std::string binarySubtraction(const std::string& binaryNumber1, const std::string& binaryNumber2) {
    // Отрицаем B (инвертируем биты и добавляем 1)
    std::string negatedBinaryNumber2 = binaryNumber2;
    for (char& bit : negatedBinaryNumber2) {
        bit = (bit == '0') ? '1' : '0';
    }
    negatedBinaryNumber2 = binaryAddition(negatedBinaryNumber2, FORMAT_8_BIT_BINARY); // Добавляем 1 (дополнительный код)
    return binaryAddition(binaryNumber1, negatedBinaryNumber2); // A + (-B)
}

// Функция для умножения двух чисел в прямом коде
int binaryMultiplication(int number1, int number2) {
    // Переводим числа в двоичный прямой код
    std::string binaryNumber1 = toDirectCode(number1, MAX_BINARY_LENGTH);
    std::string binaryNumber2 = toDirectCode(number2, MAX_BINARY_LENGTH);

    // Убираем знаковый бит (работаем с модулями чисел)
    binaryNumber1 = binaryNumber1.substr(1);
    binaryNumber2 = binaryNumber2.substr(1);

    // Переводим двоичные строки в числа
    int a = binaryToDecimal("0" + binaryNumber1); // Добавляем 0 для положительного числа
    int b = binaryToDecimal("0" + binaryNumber2);

    // Выполняем умножение
    int result = a * b;

    // Определяем знак результата
    if ((number1 < 0 && number2 > 0) || (number1 > 0 && number2 < 0)) {
        result = -result; // Результат отрицательный
    }

    return result;
}

// Функция для деления двух чисел в прямом коде
double binaryDivision(int number1, int number2) {
    if (number2 == 0) {
        std::cerr << "Ошибка: деление на ноль!\n";
        return 0;
    }

    // Переводим числа в двоичный прямой код
    std::string binaryNumber1 = toDirectCode(number1, MAX_BINARY_LENGTH);
    std::string binaryNumber2 = toDirectCode(number2, MAX_BINARY_LENGTH);

    // Убираем знаковый бит (работаем с модулями чисел)
    binaryNumber1 = binaryNumber1.substr(1);
    binaryNumber2 = binaryNumber2.substr(1);

    // Переводим двоичные строки в числа
    int a = binaryToDecimal("0" + binaryNumber1); // Добавляем 0 для положительного числа
    int b = binaryToDecimal("0" + binaryNumber2);

    // Выполняем деление
    double result = static_cast<double>(a) / b;

    // Определяем знак результата
    if ((number1 < 0 && number2 > 0) || (number1 > 0 && number2 < 0)) {
        result = -result; // Результат отрицательный
    }

    return result;
}

std::string floatToIEEE754(float number) {
    // Определяем знак
    uint32_t sign = (number < 0) ? 1 : 0;
    number = std::abs(number);

    // Нормализуем число
    int exponent;
    float mantissa = std::frexp(number, &exponent); // Разделяем на мантиссу и порядок
    mantissa = mantissa * 2 - 1; // Приводим мантиссу к виду 1.xxxxx
    exponent -= 1; // Корректируем порядок

    // Преобразуем порядок в смещённый вид (exponent bias = 127)
    uint32_t biasedExponent = exponent + EXPONENT_BIAS;

    // Преобразуем мантиссу в 23 бита
    uint32_t mantissaBits = 0;
    for (int bitIndex = 0; bitIndex < MANTISSA_BITS; ++bitIndex) {
        mantissa *= 2;
        mantissaBits <<= 1;
        if (mantissa >= 1) {
            mantissaBits |= 1;
            mantissa -= 1;
        }
    }

    // Собираем IEEE-754 представление
    uint32_t ieee754 = (sign << 31) | (biasedExponent << MANTISSA_BITS) | mantissaBits;

    // Преобразуем в двоичную строку
    std::string binaryRepresentation;
    for (int bitIndex = 31; bitIndex >= 0; --bitIndex) {
        binaryRepresentation += ((ieee754 >> bitIndex) & 1) ? '1' : '0';
    }

    return binaryRepresentation;
}

// Функция для сложения двух чисел с плавающей точкой в IEEE-754
float floatAddition(float number1, float number2) {
    // Переводим числа в IEEE-754
    std::string ieeeNumber1 = floatToIEEE754(number1);
    std::string ieeeNumber2 = floatToIEEE754(number2);

    // Извлекаем знаки, порядки и мантиссы
    uint32_t sign1 = ieeeNumber1[0] - '0';
    uint32_t sign2 = ieeeNumber2[0] - '0';

    uint32_t biasedExponent1 = 0;
    uint32_t biasedExponent2 = 0;
    for (int bitIndex = 1; bitIndex <= MAX_BINARY_LENGTH; ++bitIndex) {
        biasedExponent1 = (biasedExponent1 << 1) | (ieeeNumber1[bitIndex] - '0');
        biasedExponent2 = (biasedExponent2 << 1) | (ieeeNumber2[bitIndex] - '0');
    }

    uint32_t mantissa1 = 0;
    uint32_t mantissa2 = 0;
    for (int bitIndex = 9; bitIndex <= 31; ++bitIndex) {
        mantissa1 = (mantissa1 << 1) | (ieeeNumber1[bitIndex] - '0');
        mantissa2 = (mantissa2 << 1) | (ieeeNumber2[bitIndex] - '0');
    }

    // Добавляем ведущую единицу к мантиссам
    mantissa1 |= (1 << MANTISSA_BITS);
    mantissa2 |= (1 << MANTISSA_BITS);

    // Выравниваем порядки
    if (biasedExponent1 < biasedExponent2) {
        mantissa1 >>= (biasedExponent2 - biasedExponent1);
        biasedExponent1 = biasedExponent2;
    }
    else if (biasedExponent1 > biasedExponent2) {
        mantissa2 >>= (biasedExponent1 - biasedExponent2);
        biasedExponent2 = biasedExponent1;
    }

    // Складываем мантиссы
    uint32_t mantissaResult = mantissa1 + mantissa2;

    // Нормализуем результат
    if (mantissaResult & (1 << 24)) {
        mantissaResult >>= 1;
        biasedExponent1 += 1;
    }

    // Убираем ведущую единицу
    mantissaResult &= ~(1 << MANTISSA_BITS);

    // Собираем результат
    uint32_t result = (sign1 << 31) | (biasedExponent1 << MANTISSA_BITS) | mantissaResult;

    // Преобразуем результат обратно в float
    union {
        uint32_t i;
        float f;
    } converter;
    converter.i = result;
    return converter.f;
}

void showMenu() {
    std::cout << "\n===     Меню    ===\n";
    std::cout << "1. Вывод кодов числа\n";
    std::cout << "2. Сложение\n";
    std::cout << "3. Вычитание\n";
    std::cout << "4. Умножение\n";
    std::cout << "5. Деление\n";
    std::cout << "6. Сложение чисел с плавающей точкой (IEEE-754)\n";
    std::cout << "7. Выход\n";
    std::cout << "Ваш выбор: ";
}

int main() {
    setlocale(LC_ALL, "Russian"); 

    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;

        if (choice == 7) {
            std::cout << "\n=== Завершение работы программы ===\n";
            break;
        }

        if (choice < 1 || choice > 7) {
            std::cout << "\nОшибка: неверный выбор! Пожалуйста, выберите от 1 до 7.\n";
            continue;
        }

        if (choice == 1) {
            // Вывод кодов числа
            int number;
            std::cout << "\n=== Вывод кодов числа ===\n";
            std::cout << "Введите десятичное число: ";
            std::cin >> number;

            std::cout << "Число введено: " << number << "\n";
            std::cout << "Прямой код: [" << (number < 0 ? "1" : "0") << " " << toDirectCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number < 0 ? "1" : "0") << " " << toInverseCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number < 0 ? "1" : "0") << " " << toComplementCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 2) {
            // Сложение
            int number1, number2;
            std::cout << "\n=== Сложение ===\n";
            std::cout << "Ввод числа №1: ";
            std::cin >> number1;
            std::cout << "Число введено: " << number1 << "\n";
            std::cout << "Прямой код: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "Ввод числа №2: ";
            std::cin >> number2;
            std::cout << "Число введено: " << number2 << "\n";
            std::cout << "Прямой код: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::string complement1 = toComplementCode(number1, MAX_BINARY_LENGTH);
            std::string complement2 = toComplementCode(number2, MAX_BINARY_LENGTH);
            std::string result = binaryAddition(complement1, complement2);

            // Обрезаем результат до 8 бит
            if (result.length() > MAX_BINARY_LENGTH) {
                result = result.substr(result.length() - MAX_BINARY_LENGTH);
            }

            // Переводим результат обратно в десятичное число
            int resultDecimal = binaryToDecimal(result);

            // Выводим результат
            std::cout << "\nРезультат: " << resultDecimal << "\n";
            std::cout << "Прямой код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toDirectCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toInverseCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toComplementCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 3) {
            // Вычитание
            int number1, number2;
            std::cout << "\n=== Вычитание ===\n";
            std::cout << "Ввод числа №1: ";
            std::cin >> number1;
            std::cout << "Число введено: " << number1 << "\n";
            std::cout << "Прямой код: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "Ввод числа №2: ";
            std::cin >> number2;
            std::cout << "Число введено: " << number2 << "\n";
            std::cout << "Прямой код: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::string complement1 = toComplementCode(number1, MAX_BINARY_LENGTH);
            std::string complement2 = toComplementCode(number2, MAX_BINARY_LENGTH);
            std::string result = binarySubtraction(complement1, complement2);

            // Обрезаем результат до 8 бит
            if (result.length() > MAX_BINARY_LENGTH) {
                result = result.substr(result.length() - MAX_BINARY_LENGTH);
            }

            // Переводим результат обратно в десятичное число
            int resultDecimal = binaryToDecimal(result);

            // Выводим результат
            std::cout << "\nРезультат: " << resultDecimal << "\n";
            std::cout << "Прямой код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toDirectCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toInverseCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (resultDecimal < 0 ? "1" : "0") << " " << toComplementCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 4) {
            // Умножение
            int number1, number2;
            std::cout << "\n=== Умножение ===\n";
            std::cout << "Ввод числа №1: ";
            std::cin >> number1;
            std::cout << "Число введено: " << number1 << "\n";
            std::cout << "Прямой код: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "Ввод числа №2: ";
            std::cin >> number2;
            std::cout << "Число введено: " << number2 << "\n";
            std::cout << "Прямой код: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            int result = binaryMultiplication(number1, number2);
            std::cout << "\nРезультат: " << result << "\n";
            std::cout << "Прямой код: [" << (result < 0 ? "1" : "0") << " " << toDirectCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (result < 0 ? "1" : "0") << " " << toInverseCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (result < 0 ? "1" : "0") << " " << toComplementCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 5) {
            // Деление
            int number1, number2;
            std::cout << "\n=== Деление ===\n";
            std::cout << "Ввод числа №1: ";
            std::cin >> number1;
            std::cout << "Число введено: " << number1 << "\n";
            std::cout << "Прямой код: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "Ввод числа №2: ";
            std::cin >> number2;
            std::cout << "Число введено: " << number2 << "\n";
            std::cout << "Прямой код: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Обратный код: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "Дополнительный код: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            double result = binaryDivision(number1, number2);
            std::cout << "\nРезультат: " << std::fixed << std::setprecision(5) << result << "\n";
        }
        else if (choice == 6) {
            float number1, number2;
            std::cout << "\n=== Сложение чисел с плавающей точкой (IEEE-754) ===\n";
            std::cout << "Ввод числа №1: ";
            std::cin >> number1;
            std::cout << "Число введено: " << number1 << "\n";
            std::cout << "IEEE-754 представление: " << floatToIEEE754(number1) << "\n";

            std::cout << "Ввод числа №2: ";
            std::cin >> number2;
            std::cout << "Число введено: " << number2 << "\n";
            std::cout << "IEEE-754 представление: " << floatToIEEE754(number2) << "\n";

            // Выполняем сложение
            float result = floatAddition(number1, number2);

            std::cout << "\nРезультат: " << result << "\n";
            std::cout << "IEEE-754 представление: " << floatToIEEE754(result) << "\n";
        }
    }

    return 0;
}