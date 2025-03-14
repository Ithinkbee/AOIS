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

// ������� ��� �������� ����� � �������� ������ ���
std::string toDirectCode(int number, int bits = MAX_BINARY_LENGTH) {
    std::string binaryRepresentation;
    bool isNegative = number < 0;
    number = abs(number); // �������� � ������������� ������

    // ��������� ����� � �������� ���
    for (int bitPosition = bits - 1; bitPosition >= 0; --bitPosition) {
        binaryRepresentation += ((number >> bitPosition) & 1) ? '1' : '0';
    }

    // ���� ����� �������������, ������������� �������� ���
    if (isNegative) {
        binaryRepresentation[0] = NEGATIVE_SIGN_BIT;
    }

    return binaryRepresentation;
}

// ������� ��� �������� ����� � �������� �������� ���
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

// ������� ��� �������� ����� � �������� �������������� ���
std::string toComplementCode(int number, int bits = MAX_BINARY_LENGTH) {
    std::string inverseCode = toInverseCode(number, bits);
    if (number >= 0) {
        return inverseCode;
    }
    // ��������� 1 � ��������� ����
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

// ������� ��� �������� ���� �������� �����
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

// ������� ��� �������� ��������� ����� � ����������
int binaryToDecimal(const std::string& binaryNumber) {
    int decimalNumber = 0;
    int power = 1;
    for (int bitIndex = binaryNumber.length() - 1; bitIndex >= 0; --bitIndex) {
        if (binaryNumber[bitIndex] == '1') {
            decimalNumber += power;
        }
        power *= 2;
    }
    // ���� ����� ������������� (�������� ��� ����������)
    if (binaryNumber[0] == NEGATIVE_SIGN_BIT) {
        decimalNumber -= power;
    }
    return decimalNumber;
}

// ������� ��� ���������
std::string binarySubtraction(const std::string& binaryNumber1, const std::string& binaryNumber2) {
    // �������� B (����������� ���� � ��������� 1)
    std::string negatedBinaryNumber2 = binaryNumber2;
    for (char& bit : negatedBinaryNumber2) {
        bit = (bit == '0') ? '1' : '0';
    }
    negatedBinaryNumber2 = binaryAddition(negatedBinaryNumber2, FORMAT_8_BIT_BINARY); // ��������� 1 (�������������� ���)
    return binaryAddition(binaryNumber1, negatedBinaryNumber2); // A + (-B)
}

// ������� ��� ��������� ���� ����� � ������ ����
int binaryMultiplication(int number1, int number2) {
    // ��������� ����� � �������� ������ ���
    std::string binaryNumber1 = toDirectCode(number1, MAX_BINARY_LENGTH);
    std::string binaryNumber2 = toDirectCode(number2, MAX_BINARY_LENGTH);

    // ������� �������� ��� (�������� � �������� �����)
    binaryNumber1 = binaryNumber1.substr(1);
    binaryNumber2 = binaryNumber2.substr(1);

    // ��������� �������� ������ � �����
    int a = binaryToDecimal("0" + binaryNumber1); // ��������� 0 ��� �������������� �����
    int b = binaryToDecimal("0" + binaryNumber2);

    // ��������� ���������
    int result = a * b;

    // ���������� ���� ����������
    if ((number1 < 0 && number2 > 0) || (number1 > 0 && number2 < 0)) {
        result = -result; // ��������� �������������
    }

    return result;
}

// ������� ��� ������� ���� ����� � ������ ����
double binaryDivision(int number1, int number2) {
    if (number2 == 0) {
        std::cerr << "������: ������� �� ����!\n";
        return 0;
    }

    // ��������� ����� � �������� ������ ���
    std::string binaryNumber1 = toDirectCode(number1, MAX_BINARY_LENGTH);
    std::string binaryNumber2 = toDirectCode(number2, MAX_BINARY_LENGTH);

    // ������� �������� ��� (�������� � �������� �����)
    binaryNumber1 = binaryNumber1.substr(1);
    binaryNumber2 = binaryNumber2.substr(1);

    // ��������� �������� ������ � �����
    int a = binaryToDecimal("0" + binaryNumber1); // ��������� 0 ��� �������������� �����
    int b = binaryToDecimal("0" + binaryNumber2);

    // ��������� �������
    double result = static_cast<double>(a) / b;

    // ���������� ���� ����������
    if ((number1 < 0 && number2 > 0) || (number1 > 0 && number2 < 0)) {
        result = -result; // ��������� �������������
    }

    return result;
}

std::string floatToIEEE754(float number) {
    // ���������� ����
    uint32_t sign = (number < 0) ? 1 : 0;
    number = std::abs(number);

    // ����������� �����
    int exponent;
    float mantissa = std::frexp(number, &exponent); // ��������� �� �������� � �������
    mantissa = mantissa * 2 - 1; // �������� �������� � ���� 1.xxxxx
    exponent -= 1; // ������������ �������

    // ����������� ������� � ��������� ��� (exponent bias = 127)
    uint32_t biasedExponent = exponent + EXPONENT_BIAS;

    // ����������� �������� � 23 ����
    uint32_t mantissaBits = 0;
    for (int bitIndex = 0; bitIndex < MANTISSA_BITS; ++bitIndex) {
        mantissa *= 2;
        mantissaBits <<= 1;
        if (mantissa >= 1) {
            mantissaBits |= 1;
            mantissa -= 1;
        }
    }

    // �������� IEEE-754 �������������
    uint32_t ieee754 = (sign << 31) | (biasedExponent << MANTISSA_BITS) | mantissaBits;

    // ����������� � �������� ������
    std::string binaryRepresentation;
    for (int bitIndex = 31; bitIndex >= 0; --bitIndex) {
        binaryRepresentation += ((ieee754 >> bitIndex) & 1) ? '1' : '0';
    }

    return binaryRepresentation;
}

// ������� ��� �������� ���� ����� � ��������� ������ � IEEE-754
float floatAddition(float number1, float number2) {
    // ��������� ����� � IEEE-754
    std::string ieeeNumber1 = floatToIEEE754(number1);
    std::string ieeeNumber2 = floatToIEEE754(number2);

    // ��������� �����, ������� � ��������
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

    // ��������� ������� ������� � ���������
    mantissa1 |= (1 << MANTISSA_BITS);
    mantissa2 |= (1 << MANTISSA_BITS);

    // ����������� �������
    if (biasedExponent1 < biasedExponent2) {
        mantissa1 >>= (biasedExponent2 - biasedExponent1);
        biasedExponent1 = biasedExponent2;
    }
    else if (biasedExponent1 > biasedExponent2) {
        mantissa2 >>= (biasedExponent1 - biasedExponent2);
        biasedExponent2 = biasedExponent1;
    }

    // ���������� ��������
    uint32_t mantissaResult = mantissa1 + mantissa2;

    // ����������� ���������
    if (mantissaResult & (1 << 24)) {
        mantissaResult >>= 1;
        biasedExponent1 += 1;
    }

    // ������� ������� �������
    mantissaResult &= ~(1 << MANTISSA_BITS);

    // �������� ���������
    uint32_t result = (sign1 << 31) | (biasedExponent1 << MANTISSA_BITS) | mantissaResult;

    // ����������� ��������� ������� � float
    union {
        uint32_t i;
        float f;
    } converter;
    converter.i = result;
    return converter.f;
}

void showMenu() {
    std::cout << "\n===     ����    ===\n";
    std::cout << "1. ����� ����� �����\n";
    std::cout << "2. ��������\n";
    std::cout << "3. ���������\n";
    std::cout << "4. ���������\n";
    std::cout << "5. �������\n";
    std::cout << "6. �������� ����� � ��������� ������ (IEEE-754)\n";
    std::cout << "7. �����\n";
    std::cout << "��� �����: ";
}

int main() {
    setlocale(LC_ALL, "Russian"); 

    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;

        if (choice == 7) {
            std::cout << "\n=== ���������� ������ ��������� ===\n";
            break;
        }

        if (choice < 1 || choice > 7) {
            std::cout << "\n������: �������� �����! ����������, �������� �� 1 �� 7.\n";
            continue;
        }

        if (choice == 1) {
            // ����� ����� �����
            int number;
            std::cout << "\n=== ����� ����� ����� ===\n";
            std::cout << "������� ���������� �����: ";
            std::cin >> number;

            std::cout << "����� �������: " << number << "\n";
            std::cout << "������ ���: [" << (number < 0 ? "1" : "0") << " " << toDirectCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number < 0 ? "1" : "0") << " " << toInverseCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number < 0 ? "1" : "0") << " " << toComplementCode(number, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 2) {
            // ��������
            int number1, number2;
            std::cout << "\n=== �������� ===\n";
            std::cout << "���� ����� �1: ";
            std::cin >> number1;
            std::cout << "����� �������: " << number1 << "\n";
            std::cout << "������ ���: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "���� ����� �2: ";
            std::cin >> number2;
            std::cout << "����� �������: " << number2 << "\n";
            std::cout << "������ ���: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::string complement1 = toComplementCode(number1, MAX_BINARY_LENGTH);
            std::string complement2 = toComplementCode(number2, MAX_BINARY_LENGTH);
            std::string result = binaryAddition(complement1, complement2);

            // �������� ��������� �� 8 ���
            if (result.length() > MAX_BINARY_LENGTH) {
                result = result.substr(result.length() - MAX_BINARY_LENGTH);
            }

            // ��������� ��������� ������� � ���������� �����
            int resultDecimal = binaryToDecimal(result);

            // ������� ���������
            std::cout << "\n���������: " << resultDecimal << "\n";
            std::cout << "������ ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toDirectCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toInverseCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toComplementCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 3) {
            // ���������
            int number1, number2;
            std::cout << "\n=== ��������� ===\n";
            std::cout << "���� ����� �1: ";
            std::cin >> number1;
            std::cout << "����� �������: " << number1 << "\n";
            std::cout << "������ ���: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "���� ����� �2: ";
            std::cin >> number2;
            std::cout << "����� �������: " << number2 << "\n";
            std::cout << "������ ���: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::string complement1 = toComplementCode(number1, MAX_BINARY_LENGTH);
            std::string complement2 = toComplementCode(number2, MAX_BINARY_LENGTH);
            std::string result = binarySubtraction(complement1, complement2);

            // �������� ��������� �� 8 ���
            if (result.length() > MAX_BINARY_LENGTH) {
                result = result.substr(result.length() - MAX_BINARY_LENGTH);
            }

            // ��������� ��������� ������� � ���������� �����
            int resultDecimal = binaryToDecimal(result);

            // ������� ���������
            std::cout << "\n���������: " << resultDecimal << "\n";
            std::cout << "������ ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toDirectCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toInverseCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (resultDecimal < 0 ? "1" : "0") << " " << toComplementCode(resultDecimal, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 4) {
            // ���������
            int number1, number2;
            std::cout << "\n=== ��������� ===\n";
            std::cout << "���� ����� �1: ";
            std::cin >> number1;
            std::cout << "����� �������: " << number1 << "\n";
            std::cout << "������ ���: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "���� ����� �2: ";
            std::cin >> number2;
            std::cout << "����� �������: " << number2 << "\n";
            std::cout << "������ ���: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            int result = binaryMultiplication(number1, number2);
            std::cout << "\n���������: " << result << "\n";
            std::cout << "������ ���: [" << (result < 0 ? "1" : "0") << " " << toDirectCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (result < 0 ? "1" : "0") << " " << toInverseCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (result < 0 ? "1" : "0") << " " << toComplementCode(result, MAX_BINARY_LENGTH).substr(1) << "]\n";
        }
        else if (choice == 5) {
            // �������
            int number1, number2;
            std::cout << "\n=== ������� ===\n";
            std::cout << "���� ����� �1: ";
            std::cin >> number1;
            std::cout << "����� �������: " << number1 << "\n";
            std::cout << "������ ���: [" << (number1 < 0 ? "1" : "0") << " " << toDirectCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toInverseCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number1 < 0 ? "1" : "0") << " " << toComplementCode(number1, MAX_BINARY_LENGTH).substr(1) << "]\n";

            std::cout << "���� ����� �2: ";
            std::cin >> number2;
            std::cout << "����� �������: " << number2 << "\n";
            std::cout << "������ ���: [" << (number2 < 0 ? "1" : "0") << " " << toDirectCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toInverseCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";
            std::cout << "�������������� ���: [" << (number2 < 0 ? "1" : "0") << " " << toComplementCode(number2, MAX_BINARY_LENGTH).substr(1) << "]\n";

            double result = binaryDivision(number1, number2);
            std::cout << "\n���������: " << std::fixed << std::setprecision(5) << result << "\n";
        }
        else if (choice == 6) {
            float number1, number2;
            std::cout << "\n=== �������� ����� � ��������� ������ (IEEE-754) ===\n";
            std::cout << "���� ����� �1: ";
            std::cin >> number1;
            std::cout << "����� �������: " << number1 << "\n";
            std::cout << "IEEE-754 �������������: " << floatToIEEE754(number1) << "\n";

            std::cout << "���� ����� �2: ";
            std::cin >> number2;
            std::cout << "����� �������: " << number2 << "\n";
            std::cout << "IEEE-754 �������������: " << floatToIEEE754(number2) << "\n";

            // ��������� ��������
            float result = floatAddition(number1, number2);

            std::cout << "\n���������: " << result << "\n";
            std::cout << "IEEE-754 �������������: " << floatToIEEE754(result) << "\n";
        }
    }

    return 0;
}