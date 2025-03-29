import unittest
from convert import *
from arithmetics import *

class TestBinaryConverter(unittest.TestCase):
    def test_direct_code(self):
        self.assertEqual(to_direct_code(5), "00000101")
        self.assertEqual(to_direct_code(-5), "10000101")
        self.assertEqual(to_direct_code(127), "01111111")
        self.assertEqual(to_direct_code(-127), "11111111")
        self.assertEqual(to_direct_code(0), "00000000")
    
    def test_inverse_code(self):
        self.assertEqual(to_inverse_code(5), "00000101")
        self.assertEqual(to_inverse_code(-5), "11111010")
        self.assertEqual(to_inverse_code(127), "01111111")
        self.assertEqual(to_inverse_code(-127), "10000000")
        self.assertEqual(to_inverse_code(0), "00000000")
    
    def test_complement_code(self):
        self.assertEqual(to_complement_code(5), "00000101")
        self.assertEqual(to_complement_code(-5), "11111011")
        self.assertEqual(to_complement_code(127), "01111111")
        self.assertEqual(to_complement_code(-127), "10000001")
        self.assertEqual(to_complement_code(0), "00000000")

    def test_ieee_754(self):
        self.assertEqual(float_to_ieee_754(5.25), "01000000101010000000000000000000")
        self.assertEqual(float_to_ieee_754(-3.625), "11000000011010000000000000000000")
        self.assertEqual(float_to_ieee_754(0.0), "00000000000000000000000000000000")
        self.assertEqual(float_to_ieee_754(127.5), "01000010111111110000000000000000")
        self.assertEqual(float_to_ieee_754(-0.15625), "10111110001000000000000000000000")

class TestArithmetics(unittest.TestCase):
    def test_add_complement(self):
        self.assertEqual(add_complement(5, 3), 8)
        self.assertEqual(add_complement(5, -3), 2)
        self.assertEqual(add_complement(-5, -3), -8)
        self.assertEqual(add_complement(127, 1), -128) 
        self.assertEqual(add_complement(-128, -1), 127)  

    def test_subtract_complement(self):
        self.assertEqual(subtract_complement(5, 3), 2)
        self.assertEqual(subtract_complement(5, -3), 8)
        self.assertEqual(subtract_complement(-5, -3), -2)
        self.assertEqual(subtract_complement(127, -1), -128) 
        self.assertEqual(subtract_complement(-128, 1), 127)
        
    def test_multiply_direct(self):
        self.assertEqual(multiply_direct(5, 3), 15)
        self.assertEqual(multiply_direct(-6, 4), -24)
        self.assertEqual(multiply_direct(7, -7), -49)
        self.assertEqual(multiply_direct(-1, -3), 3)
        self.assertEqual(multiply_direct(50, 0), 0)
        with self.assertRaises(ValueError):
            multiply_direct(64, 2)  # 128 > 127

    def test_divide_direct(self):
        self.assertAlmostEqual(divide_direct(10, 3), 3.31250, places=5)
        self.assertAlmostEqual(divide_direct(-10, 3), -3.31250, places=5)
        self.assertAlmostEqual(divide_direct(1, 2), 0.5, places=5)
        self.assertAlmostEqual(divide_direct(127, 64),  1.96875, places=5)
        with self.assertRaises(ZeroDivisionError):
            divide_direct(10, 0)

    def test_add_ieee754_simple(self):
        # 1.5 + 0.75 = 2.25
        self.assertEqual(add_ieee754_simple(1.5, 0.75), "01000000000100000000000000000000")
        # 2.625 + 13.25 = 15.875
        self.assertEqual(add_ieee754_simple(2.625, 13.25), "01000001011111100000000000000000")
        # 10.0 + 1.5 = 11.5
        self.assertEqual(add_ieee754_simple(10.0, 1.5), "01000001001110000000000000000000")
        # 12.625 + 4.0 = 16.625
        self.assertEqual(add_ieee754_simple(12.625, 4.0), "01000001100001010000000000000000")

if __name__ == "__main__":
    unittest.main()

