import unittest
from hashtable import HashTable
from io import StringIO
import sys

class TestHashTable(unittest.TestCase):
    def setUp(self):
        self.ht = HashTable(size=5)

    def test_insert_and_search(self):
        self.ht.insert("a", "apple")
        self.ht.insert("b", "banana")
        self.assertEqual(self.ht.search("a"), "apple")
        self.assertEqual(self.ht.search("b"), "banana")

    def test_update_existing_key(self):
        self.ht.insert("a", "apple")
        self.ht.insert("a", "apricot")
        self.assertEqual(self.ht.search("a"), "apricot")

    def test_delete_existing_key(self):
        self.ht.insert("a", "apple")
        result = self.ht.delete("a")
        self.assertTrue(result)
        self.assertIsNone(self.ht.search("a"))

    def test_delete_nonexistent_key(self):
        result = self.ht.delete("nonexistent")
        self.assertFalse(result)

    def test_collision_resolution(self):
        keys = ["a", "f", "k"]  
        values = ["alpha", "foxtrot", "kilo"]
        for key, val in zip(keys, values):
            self.ht.insert(key, val)

        for key, val in zip(keys, values):
            self.assertEqual(self.ht.search(key), val)

    def test_to_dict(self):
        self.ht.insert("a", "apple")
        self.ht.insert("b", "banana")
        self.assertEqual(self.ht.to_dict(), {"a": "apple", "b": "banana"})

    def test_from_dict(self):
        data = {
            "x": "x-ray",
            "y": "yellow",
            "z": "zebra"
        }
        self.ht.from_dict(data)
        self.assertEqual(self.ht.search("x"), "x-ray")
        self.assertEqual(self.ht.search("y"), "yellow")
        self.assertEqual(self.ht.search("z"), "zebra")

    def test_overwrite_on_from_dict(self):
        self.ht.insert("x", "wrong")
        self.ht.from_dict({"x": "correct"})
        self.assertEqual(self.ht.search("x"), "correct")

    def test_to_dict_skips_deleted(self):
        self.ht.insert("a", "alpha")
        self.ht.insert("b", "bravo")
        self.ht.delete("a")
        result = self.ht.to_dict()
        self.assertNotIn("a", result)
        self.assertIn("b", result)

    def test_display_output(self):
        self.ht.insert("a", "alpha")
        self.ht.insert("f", "foxtrot") 

        captured_output = StringIO()
        sys.stdout = captured_output 

        self.ht.display()

        sys.stdout = sys.__stdout__  
        output = captured_output.getvalue()

        self.assertIn("alpha", output)
        self.assertIn("foxtrot", output)
        self.assertIn("empty", output or "empty" in output)

    def test_display_with_deleted_entry(self):
        self.ht.insert("a", "alpha")
        self.ht.delete("a")

        captured_output = StringIO()
        sys.stdout = captured_output

        self.ht.display()

        sys.stdout = sys.__stdout__
        output = captured_output.getvalue()
        self.assertIn("deleted", output or "deleted" in output)

if __name__ == '__main__':
    unittest.main()
