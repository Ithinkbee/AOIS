from global_values import *

def manual_binary(n: int, bits: int) -> str:
    result = []
    for _ in range(bits):
        result.append(str(n % BINARY_BASE))
        n //= BINARY_BASE
    return ''.join(result[::-1])


def to_binary_8bit(n: int) -> str:
    min_val = -(2 ** (BIT_LENGTH_DEFAULT - 1))
    max_val = (2 ** (BIT_LENGTH_DEFAULT - 1)) - 1
    
    if not (min_val <= n <= max_val):
        raise ValueError(f"Value must fit in [{min_val}, {max_val}] for {BIT_LENGTH_DEFAULT}-bit representation")
    
    sign_bit = '0' if n >= 0 else '1'
    magnitude = abs(n)
    binary_magnitude = manual_binary(magnitude, BIT_LENGTH_DEFAULT - 1)
    
    return sign_bit + binary_magnitude


def to_direct_code(n: int) -> str:
    return to_binary_8bit(n)


def to_inverse_code(n: int) -> str:
    if n >= 0:
        return to_binary_8bit(n)
    
    direct_code = to_binary_8bit(n)
    inverted_part = ''.join('1' if bit == '0' else '0' for bit in direct_code[1:])
    
    return direct_code[0] + inverted_part


def add_one(binary_str: str) -> str:
    binary_list = list(binary_str)
    carry = 1
    for i in range(len(binary_list) - 1, -1, -1):
        if binary_list[i] == '1' and carry == 1:
            binary_list[i] = '0'
        elif binary_list[i] == '0' and carry == 1:
            binary_list[i] = '1'
            carry = 0
    return ''.join(binary_list)


def to_complement_code(n: int) -> str:
    if n >= 0:
        return to_binary_8bit(n)
    
    inverse_code = to_inverse_code(n)
    complement_part = add_one(inverse_code[1:])
    
    return inverse_code[0] + complement_part


def float_to_ieee_754(num: float) -> str:
    if num == 0.0:
        return IEEE754_ZERO
    
    sign_bit = "0" if num >= 0 else "1"
    num = abs(num)
    
    int_part = int(num)
    frac_part = num - int_part
    
    int_bin = manual_binary(int_part, IEEE754_TOTAL_BITS).lstrip('0') if int_part != 0 else ""
    
    frac_bin = ""
    if frac_part != 0:
        current_frac = frac_part
        while current_frac != 0 and len(frac_bin) < IEEE754_SIGNIFICAND_LENGTH + 1:
            current_frac *= BINARY_BASE
            bit = int(current_frac)
            frac_bin += str(bit)
            current_frac -= bit
    
    full_bin = int_bin + frac_bin
    
    if int_part > 0:
        exponent = len(int_bin) - 1
        mantissa = (int_bin[1:] + frac_bin)[:IEEE754_SIGNIFICAND_LENGTH]
    else:
        first_one_pos = full_bin.find(IEEE754_HIDDEN_BIT)
        if first_one_pos == -1:
            return sign_bit + "0" * (IEEE754_TOTAL_BITS - 1)
        exponent = -(first_one_pos + 1)
        mantissa = full_bin[first_one_pos+1:first_one_pos+1+IEEE754_SIGNIFICAND_LENGTH]
    
    exponent += IEEE754_BIAS
    if exponent <= 0:
        return sign_bit + "0" * IEEE754_EXPONENT_LENGTH + "0" * IEEE754_SIGNIFICAND_LENGTH
    
    exponent_bin = manual_binary(exponent, IEEE754_EXPONENT_LENGTH)
    
    mantissa = mantissa.ljust(IEEE754_SIGNIFICAND_LENGTH, '0')[:IEEE754_SIGNIFICAND_LENGTH]
    
    return sign_bit + exponent_bin + mantissa


def complement_to_decimal(binary: str, bits: int) -> int:
    value = 0
    for i, bit in enumerate(binary[1:]):
        value += int(bit) * (BINARY_BASE ** (bits - 2 - i))
    
    if binary[0] == '1':
        value = value - (1 << (bits - 1))
    
    return value


def multiply_unsigned(a: int, b: int) -> int:
    result = 0
    shift = 0
    while b > 0:
        if b % BINARY_BASE == 1:
            result += a << shift
        b = b // BINARY_BASE
        shift += 1
    return result


def ieee754_to_float(ieee: str) -> float:
    if ieee == IEEE754_ZERO:
        return 0.0
    
    sign_bit = -1 if ieee[0] == IEEE754_HIDDEN_BIT else 1
    exponent = int(ieee[1:9], 2) - IEEE754_BIAS
    mantissa_bits = ieee[9:]
    
    if exponent == -IEEE754_BIAS:
        mantissa = int(mantissa_bits, 2) / (1 << IEEE754_SIGNIFICAND_LENGTH)
        return sign_bit * mantissa * (2 ** (IEEE754_DENORMALIZED_EXPONENT + 1))
    
    mantissa = 1 + int(mantissa_bits, 2) / (1 << IEEE754_SIGNIFICAND_LENGTH)
    return sign_bit * mantissa * (2 ** exponent)