from processor import *

def main():
    ap = AssociativeProcessor()

    ap.print_raw_matrix()

    ap.print_memory()
    
    word_idx = 2
    column_idx = 3
    print(f"Word #{word_idx}: {''.join(map(str, ap.read_word(word_idx)))}")
    print(f"Column #{column_idx}: {''.join(map(str, ap.read_column(column_idx)))}")
    print()
    
    word1 = ap.read_word(0)
    word2 = ap.read_word(1)
    print(f"Word 0: {''.join(map(str, word1))}")
    print(f"Word 1: {''.join(map(str, word2))}")
    
    print("\nLogic operations result:")
    print(f"f1 (AND): {''.join(map(str, ap.apply_logical_function('f1', word1, word2)))}")
    print(f"f14 (NAND): {''.join(map(str, ap.apply_logical_function('f14', word1, word2)))}")
    print(f"f3 (YES x1): {''.join(map(str, ap.apply_logical_function('f3', word1)))}")
    print(f"f12 (NOT x1): {''.join(map(str, ap.apply_logical_function('f12', word1)))}")
    print()
    
    target = [0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0] 
    print(f"Target word: {''.join(map(str, target))}")
    
    nearest_above = ap.find_nearest_value(target, 'above')
    if nearest_above is not None:
        print(f"Closest above: word #{nearest_above}: {''.join(map(str, ap.read_word(nearest_above)))}")
    else:
        print("Closest above not found")
    
    nearest_below = ap.find_nearest_value(target, 'below')
    if nearest_below is not None:
        print(f"Closest below: word #{nearest_below}: {''.join(map(str, ap.read_word(nearest_below)))}")
    else:
        print("Closest below not found")
    print()
    
    v_mask = [1, 1, 1]  
    print(f"Sum of Aj and Bj for words with Vj = {''.join(map(str, v_mask))}")
    results = ap.add_fields(v_mask)
    for word_idx, new_word in results:
        print(f"Word {word_idx} updated: {''.join(map(str, new_word))}")
    
    print("\nUpdated Matrix:")
    ap.print_raw_matrix()

    print("\nUpdated Memory:")
    ap.print_memory()

if __name__ == "__main__":
    main()
