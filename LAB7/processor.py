import random

class AssociativeProcessor:
    def __init__(self, word_size=16, num_words=16):
        self.word_size = word_size
        self.num_words = num_words
        self.memory = [[0 for _ in range(word_size)] for _ in range(word_size)]
        self.initialize_vertical_memory()
        
    def initialize_vertical_memory(self):
        words = [[random.randint(0, 1) for _ in range(self.word_size)] for _ in range(self.num_words)]

        for word_idx in range(self.num_words):
            col = word_idx
            for bit_idx in range(self.word_size):
                row = (bit_idx + word_idx) % self.word_size
                self.memory[row][col] = words[word_idx][bit_idx]

    def print_raw_matrix(self):
        print("Raw Matrix View:")
        for row in self.memory:
            print("".join(map(str, row)))
        print()

    def print_memory(self):
        print("Memory:")
        for i in range(self.num_words):
            word = self.get_cyclic_column_word(i)
            print(f"Word {i:2d}: {''.join(map(str, word))}")
        print()

    def get_cyclic_column_word(self, word_idx):
        col = word_idx
        return [self.memory[(i + word_idx) % self.word_size][col] for i in range(self.word_size)]

    def read_word(self, word_idx):
        if 0 <= word_idx < self.num_words:
            return self.get_cyclic_column_word(word_idx)
        return None

    def read_column(self, column_idx):
        if 0 <= column_idx < self.word_size:
            return [self.memory[(row + column_idx) % self.word_size][row] for row in range(self.word_size)]
        return None

    def write_word(self, word_idx, data):
        if 0 <= word_idx < self.num_words and len(data) == self.word_size:
            col = word_idx
            for i in range(self.word_size):
                row = (i + word_idx) % self.word_size
                self.memory[row][col] = data[i]
            return True
        return False

    def apply_logical_function(self, func_name, column1, column2=None):
        if column2 is None:
            column2 = column1
        
        result = []
        for x1, x2 in zip(column1, column2):
            if func_name == 'f1':  # AND
                res = x1 & x2
            elif func_name == 'f14':  # NAND
                res = 1 if not (x1 & x2) else 0
            elif func_name == 'f3':  # YES x1
                res = x1
            elif func_name == 'f12':  # NOT x1
                res = 1 - x1
            else:
                res = 0
            result.append(res)
        return result

    def find_nearest_value(self, target, direction='above'):
        target_int = self.word_to_int(target)
        candidates = []
    
        for word_idx in range(self.num_words):
            word = self.read_word(word_idx)
            word_int = self.word_to_int(word)
        
            if direction == 'above' and word_int > target_int:
                candidates.append((word_idx, word_int))
            elif direction == 'below' and word_int < target_int:
                candidates.append((word_idx, word_int))
    
        if not candidates:
            return None
    
        if direction == 'above':
            min_word = min(candidates, key=lambda x: x[1])
            return min_word[0]
        else:
            max_word = max(candidates, key=lambda x: x[1])
            return max_word[0]

    def compare_words(self, word, target):
        g = 0
        l = 0
        
        for i in range(self.word_size - 1, -1, -1):
            a_i = target[i]
            s_i = word[i]
            
            new_g = g or (a_i and (not s_i) and (not l))
            new_l = l or ((not a_i) and s_i and (not g))
            
            g, l = new_g, new_l
        
        return g, l

    def word_to_int(self, word):
        return int(''.join(map(str, word)), 2)

    def add_fields(self, v_mask):
        results = []
        
        for word_idx in range(self.num_words):
            word = self.read_word(word_idx)
            v_field = word[:3]
            
            if v_field == v_mask:
                a_field = word[3:7]
                b_field = word[7:11]
                s_field = word[11:16]
                
                a_int = self.word_to_int(a_field)
                b_int = self.word_to_int(b_field)
                sum_int = a_int + b_int
                
                if sum_int >= 32:
                    sum_int = 31  
                
                sum_bits = [int(b) for b in format(sum_int, '05b')]
                new_word = v_field + a_field + b_field + sum_bits
                self.write_word(word_idx, new_word)
                results.append((word_idx, new_word))
        
        return results