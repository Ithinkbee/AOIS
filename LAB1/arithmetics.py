from convert import *
from global_values import *


def add_complement(a: int, b: int, bits: int = BIT_LENGTH_DEFAULT) -> int:
    min_val = -(2 ** (bits - 1))
    max_val = (2 ** (bits - 1)) - 1
    
    if not (min_val <= a <= max_val and min_val <= b <= max_val):
        raise ValueError(f"Numbers must fit in [{min_val}, {max_val}] for {bits}-bit representation")
    
    a_comp = to_complement_code(a)
    b_comp = to_complement_code(b)
    
    result_comp = _add_binary(a_comp, b_comp)
    
    return complement_to_decimal(result_comp, bits)


def _add_binary(a: str, b: str) -> str:
    max_len = max(len(a), len(b))
    a = a.zfill(max_len)
    b = b.zfill(max_len)
    
    result = []
    carry = 0
    
    for i in range(max_len - 1, -1, -1):
        sum_bits = int(a[i]) + int(b[i]) + carry
        result.append(str(sum_bits % BINARY_BASE))
        carry = sum_bits // BINARY_BASE
    
    return ''.join(reversed(result))


def subtract_complement(a: int, b: int, bits: int = BIT_LENGTH_DEFAULT) -> int:
    return add_complement(a, -b, bits)


def multiply_direct(a: int, b: int, bits: int = BIT_LENGTH_DEFAULT) -> int:
    min_val = -(2 ** (bits - 1) - 1)
    max_val = 2 ** (bits - 1) - 1
    
    if not (min_val <= a <= max_val and min_val <= b <= max_val):
        raise ValueError(f"Numbers must fit in [{min_val}, {max_val}] for {bits}-bit representation")
    
    sign = (a < 0) ^ (b < 0)
    
    abs_a = abs(a)
    abs_b = abs(b)
    
    result_abs = multiply_unsigned(abs_a, abs_b)
    
    max_abs = 2 ** (bits - 1) - 1
    if result_abs > max_abs:
        raise ValueError("Result overflow")
    
    return -result_abs if sign else result_abs


def divide_direct(a: int, b: int, precision: int = FRACTIONAL_PRECISION, bits: int = BIT_LENGTH_DEFAULT) -> str:
    if b == 0:
        raise ZeroDivisionError("Can't divide by zero!")

    min_val = -(2 ** (bits - 1) - 1)
    max_val = (2 ** (bits - 1) - 1)
    
    if not (min_val <= a <= max_val and min_val <= b <= max_val):
        raise ValueError(f"Numbers must fit in [{min_val}, {max_val}] for {bits}-bit representation")

    sign = (a < 0) ^ (b < 0)

    abs_a = abs(a)
    abs_b = abs(b)

    dividend_bin = bin(abs_a)[2:].zfill(bits)
    divisor_bin = bin(abs_b)[2:].zfill(bits)

    quotient = ""
    remainder = 0

    for bit in dividend_bin:
        remainder = (remainder << 1) | int(bit)
        if remainder >= abs_b:
            quotient += '1'
            remainder -= abs_b
        else:
            quotient += '0'

    quotient += '.'
    for _ in range(precision):
        remainder <<= 1
        if remainder >= abs_b:
            quotient += '1'
            remainder -= abs_b
        else:
            quotient += '0'

    if sign:
        quotient = '1' + quotient[1:]
    else:
        quotient = '0' + quotient[1:]

    return quotient


def add_ieee754_simple(a: float, b: float) -> str:
    a_ieee = float_to_ieee_754(a)
    b_ieee = float_to_ieee_754(b)

    if a_ieee == IEEE754_ZERO:
        return b_ieee
    if b_ieee == IEEE754_ZERO:
        return a_ieee

    sign_a, exponent_a, mantissa_a = _parse_ieee754(a_ieee)
    sign_b, exponent_b, mantissa_b = _parse_ieee754(b_ieee)

    shift = exponent_a - exponent_b
    if shift > 0:
        mantissa_b = _shift_mantissa(mantissa_b, shift)
        exponent = exponent_a
    elif shift < 0:
        mantissa_a = _shift_mantissa(mantissa_a, -shift)
        exponent = exponent_b
    else:
        exponent = exponent_a

    if sign_a == sign_b:
        mantissa = mantissa_a + mantissa_b
        sign = sign_a
    else:
        if mantissa_a > mantissa_b:
            mantissa = mantissa_a - mantissa_b
            sign = sign_a
        else:
            mantissa = mantissa_b - mantissa_a
            sign = sign_b

    if mantissa == 0:
        return IEEE754_ZERO
        
    while mantissa >= (BINARY_BASE << IEEE754_SIGNIFICAND_LENGTH):
        mantissa >>= 1
        exponent += 1
    while mantissa < (1 << IEEE754_SIGNIFICAND_LENGTH) and exponent > IEEE754_DENORMALIZED_EXPONENT + 1:
        mantissa <<= 1
        exponent -= 1

    if exponent > (2 ** IEEE754_EXPONENT_LENGTH - 1 - IEEE754_BIAS):
        return sign + "1" * IEEE754_EXPONENT_LENGTH + "0" * IEEE754_SIGNIFICAND_LENGTH
    if exponent < IEEE754_DENORMALIZED_EXPONENT + 1:
        return IEEE754_ZERO

    mantissa &= (1 << IEEE754_SIGNIFICAND_LENGTH) - 1

    return _build_ieee754(sign, exponent, mantissa)

def _parse_ieee754(ieee: str) -> tuple[str, int, int]:
    sign = ieee[0]
    exponent = int(ieee[1:1+IEEE754_EXPONENT_LENGTH], BINARY_BASE) - IEEE754_BIAS
    mantissa = int(ieee[1+IEEE754_EXPONENT_LENGTH:], BINARY_BASE)
    
    if exponent != IEEE754_DENORMALIZED_EXPONENT:
        mantissa |= (1 << IEEE754_SIGNIFICAND_LENGTH)
    else:
        exponent = IEEE754_DENORMALIZED_EXPONENT + 1
    
    return sign, exponent, mantissa

def _shift_mantissa(mantissa: int, shift: int) -> int:
    if shift > (IEEE754_SIGNIFICAND_LENGTH + 1):
        return 0
    return mantissa >> shift

def _build_ieee754(sign: str, exponent: int, mantissa: int) -> str:
    if exponent < IEEE754_DENORMALIZED_EXPONENT + 1:
        exponent = IEEE754_DENORMALIZED_EXPONENT
        mantissa &= (1 << IEEE754_SIGNIFICAND_LENGTH) - 1
    
    exponent += IEEE754_BIAS
    exponent = max(0, min(exponent, (1 << IEEE754_EXPONENT_LENGTH) - 1))
    
    exponent_bits = manual_binary(exponent, IEEE754_EXPONENT_LENGTH)
    mantissa_bits = manual_binary(mantissa, IEEE754_SIGNIFICAND_LENGTH)
    
    return sign + exponent_bits + mantissa_bits