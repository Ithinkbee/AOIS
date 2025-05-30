import json
from hashtable import HashTable

def main():
    ht = HashTable()

    while True:
        print("\nMenu:")
        print("1. Add or update an entry")
        print("2. Search for a key")
        print("3. Delete an entry")
        print("4. Display the hash table")
        print("5. Exit")
        print("6. Load hash table from a JSON file")
        print("7. Save hash table to a JSON file")

        choice = input("Choose an option: ")

        if choice == '1':
            key = input("Enter key: ")
            value = input("Enter value: ")
            ht.insert(key, value)
            print("Entry added or updated.")
        elif choice == '2':
            key = input("Enter key to search: ")
            value = ht.search(key)
            if value is not None:
                print(f"Found: {value}")
            else:
                print("Key not found.")
        elif choice == '3':
            key = input("Enter key to delete: ")
            if ht.delete(key):
                print("Entry deleted.")
            else:
                print("Key not found.")
        elif choice == '4':
            ht.display()
        elif choice == '5':
            print("Exiting.")
            break
        elif choice == '6':
            filename = input("Enter JSON file name to load: ")
            try:
                with open(filename, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    ht = HashTable()  
                    ht.from_dict(data)
                    print("Hash table loaded.")
            except Exception as e:
                print(f"Error loading file: {e}")
        elif choice == '7':
            filename = input("Enter JSON file name to save: ")
            try:
                with open(filename, 'w', encoding='utf-8') as f:
                    json.dump(ht.to_dict(), f, ensure_ascii=False, indent=2)
                    print("Hash table saved.")
            except Exception as e:
                print(f"Error saving file: {e}")
        else:
            print("Invalid option.")

if __name__ == "__main__":
    main()