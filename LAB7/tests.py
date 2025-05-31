import unittest
from processor import AssociativeProcessor
import random

class TestAssociativeProcessor(unittest.TestCase):
    def setUp(self):
        self.ap = AssociativeProcessor(word_size=16, num_words=16)
        random.seed(42)
        self.ap.initialize_vertical_memory()

    def test_initialization(self):
        self.assertEqual(len(self.ap.memory), 16)
        self.assertEqual(len(self.ap.memory[0]), 16)
        self.assertEqual(self.ap.word_size, 16)
        self.assertEqual(self.ap.num_words, 16)

    def test_read_write_word(self):
        test_word = [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
        self.assertTrue(self.ap.write_word(0, test_word))
        self.assertEqual(self.ap.read_word(0), test_word)

        self.assertFalse(self.ap.write_word(20, test_word))
        self.assertIsNone(self.ap.read_word(20))

    def test_logical_operations(self):
        word1 = [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
        word2 = [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]

        # AND
        and_result = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.assertEqual(self.ap.apply_logical_function('f1', word1, word2), and_result)

        # NAND
        nand_result = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        self.assertEqual(self.ap.apply_logical_function('f14', word1, word2), nand_result)

        # YES x1
        self.assertEqual(self.ap.apply_logical_function('f3', word1), word1)

        # NOT x1
        not_result = [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]
        self.assertEqual(self.ap.apply_logical_function('f12', word1), not_result)

    def test_word_comparison(self):
        word1 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]  # 1
        word2 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0]  # 2

        # word2 > word1
        g, l = self.ap.compare_words(word2, word1)
        self.assertEqual(g, 1)
        self.assertEqual(l, 0)

        # word1 < word2
        g, l = self.ap.compare_words(word1, word2)
        self.assertEqual(g, 0)
        self.assertEqual(l, 1)

        g, l = self.ap.compare_words(word1, word1.copy())
        self.assertEqual(g, 0)
        self.assertEqual(l, 0)

    def test_find_nearest_value(self):
        test_words = [
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],  # 1
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],  # 2
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],  # 4
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],  # 8
        ]
        
        for i, word in enumerate(test_words):
            self.ap.write_word(i, word)

        target = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1]  # 3

        nearest_above = self.ap.find_nearest_value(target, 'above')
        self.assertEqual(nearest_above, 2)
        self.assertEqual(self.ap.word_to_int(self.ap.read_word(nearest_above)), 4)

        nearest_below = self.ap.find_nearest_value(target, 'below')
        self.assertEqual(nearest_below, 1)
        self.assertEqual(self.ap.word_to_int(self.ap.read_word(nearest_below)), 2)

        high_target = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]  # 65535
        self.assertIsNone(self.ap.find_nearest_value(high_target, 'above'))
        
        low_target = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # 0
        self.assertIsNone(self.ap.find_nearest_value(low_target, 'below'))

    def test_add_fields(self):
        ap = AssociativeProcessor(word_size=16, num_words=16)
        ap.memory = [[0 for _ in range(16)] for _ in range(16)]  
    
        test_word1 = [1,1,1, 0,1,0,1, 0,1,0,1, 0,0,0,0,0]
        ap.write_word(0, test_word1)
    
        test_word2 = [0,0,0] + [0]*13
        ap.write_word(1, test_word2)
    
        test_word3 = [1,1,1, 1,1,1,1, 1,1,1,1, 0,0,0,0,0]
        ap.write_word(2, test_word3)
    
        results = ap.add_fields([1,1,1])
    
        self.assertEqual(len(results), 2)
    
        result_word1 = next(w for idx, w in results if idx == 0)
        result_word3 = next(w for idx, w in results if idx == 2)
    
        self.assertEqual(result_word1[:3], [1,1,1])  # Vj
        self.assertEqual(result_word1[3:7], [0,1,0,1])  # Aj (5)
        self.assertEqual(result_word1[7:11], [0,1,0,1])  # Bj (5)
        self.assertEqual(result_word1[11:16], [0,1,0,1,0])  # Sj (10)
    
        self.assertEqual(result_word3[11:16], [1,1,1,1,0])  
    
        self.assertEqual(ap.read_word(1), test_word2)

    def test_word_to_int(self):
        self.assertEqual(self.ap.word_to_int([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), 0)
        self.assertEqual(self.ap.word_to_int([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]), 1)
        self.assertEqual(self.ap.word_to_int([1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), 32768)
        self.assertEqual(self.ap.word_to_int([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]), 65535)

if __name__ == '__main__':
    unittest.main()
