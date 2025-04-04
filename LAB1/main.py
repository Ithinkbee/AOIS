from convert import *
from arithmetics import *
from global_values import *

def display_menu():
    print("\nSelect operation:")
    print("1. Integer operations")
    print("2. Floating-point operations")
    print("3. Exit")

def integer_operations():
    try:
        num1 = int(input("Enter first integer: "))
        num2 = int(input("Enter second integer: "))
        bits = int(BIT_LENGTH_DEFAULT)

        print("\nNumber representations:")
        print(f"{num1}:")
        print(f"  Direct: {to_direct_code(num1)}")
        print(f"  Inverse: {to_inverse_code(num1)}")
        print(f"  Complement: {to_complement_code(num1)}")
        
        print(f"\n{num2}:")
        print(f"  Direct: {to_direct_code(num2)}")
        print(f"  Inverse: {to_inverse_code(num2)}")
        print(f"  Complement: {to_complement_code(num2)}")

        print("\nOperation results:")
        print(f"\nAddition: {add_complement(num1, num2, bits)}")
        print(f"  Direct: {to_direct_code(add_complement(num1, num2, bits))}")
        print(f"  Inverse: {to_inverse_code(add_complement(num1, num2, bits))}")
        print(f"  Complement: {to_complement_code(add_complement(num1, num2, bits))}")

        print(f"\nSubtraction: {subtract_complement(num1, num2, bits)}")
        print(f"  Direct: {to_direct_code(subtract_complement(num1, num2, bits))}")
        print(f"  Inverse: {to_inverse_code(subtract_complement(num1, num2, bits))}")
        print(f"  Complement: {to_complement_code(subtract_complement(num1, num2, bits))}")

        if (num1 * num2) >= 128:
            print("\nMultiplication: Sorry, but the result is more than 128")
        elif (num1 * num2) <= -128:
            print("\nMultiplication: Sorry, but the result is less than -128")
        else:
            print(f"\nMultiplication: {multiply_direct(num1, num2, bits)}")
            print(f"  Direct: {to_direct_code(multiply_direct(num1, num2, bits))}")
            print(f"  Inverse: {to_inverse_code(multiply_direct(num1, num2, bits))}")
            print(f"  Complement: {to_complement_code(multiply_direct(num1, num2, bits))}")
        
        if num2 != 0:
            print(f"\nDivision: {divide_direct(num1, num2)}")
            print(f"Check: {binary_fixed_to_decimal(divide_direct(num1, num2))}")
        else:
            print("\nDivision by zero error!")
            
    except ValueError as e:
        print(f"Error: {e}")

def float_operations():
    try:
        num1 = float(input("Enter first number: "))
        num2 = float(input("Enter second number: "))

        print("\nIEEE-754 representation:")
        ieee1 = float_to_ieee_754(num1)
        ieee2 = float_to_ieee_754(num2)
        print(f"{num1} = {ieee1}")
        print(f"{num2} = {ieee2}")

        ieee_sum = add_ieee754_simple(num1, num2)
        decimal_sum = ieee754_to_float(ieee_sum)
        
        print("\nAddition result:")
        print(f"Binary: {ieee_sum}")
        print(f"Decimal: {decimal_sum}")
        print(f"Verification: {num1 + num2}")

    except Exception as e:
        print(f"Error: {e}")

def main():
    print("Binary Calculator v1.0")
    while True:
        display_menu()
        choice = input("Your choice (1-3): ").strip()
        
        if choice == "1":
            integer_operations()
        elif choice == "2":
            float_operations()
        elif choice == "3":
            print("Exiting program.")
            break
        else:
            print("Invalid input. Please try again.")

if __name__ == "__main__":
    main()
