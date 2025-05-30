
class HashTable:
    def __init__(self, size=20):
        self.size = size
        self.table = [None] * size
        self.deleted = object()

    def _hash(self, key):
        if len(key) >= 2:
            hash_value = ord(key[0]) + ord(key[1])
        elif len(key) == 1:
            hash_value = ord(key[0]) * 2
        else:
            hash_value = 0
        return hash_value % self.size

    def insert(self, key, value):
        index = self._hash(key)
        start_index = index
        while self.table[index] is not None and self.table[index] is not self.deleted:
            if self.table[index][0] == key:
                self.table[index] = (key, value)
                return
            index = (index + 1) % self.size
            if index == start_index:
                raise Exception("Hash table is full. Cannot insert new key.")
        self.table[index] = (key, value)

    def search(self, key):
        index = self._hash(key)
        start_index = index
        while self.table[index] is not None:
            if self.table[index] is not self.deleted and self.table[index][0] == key:
                return self.table[index][1]
            index = (index + 1) % self.size
            if index == start_index:
                break
        return None

    def delete(self, key):
        index = self._hash(key)
        start_index = index
        while self.table[index] is not None:
            if self.table[index] is not self.deleted and self.table[index][0] == key:
                self.table[index] = self.deleted
                return True
            index = (index + 1) % self.size
            if index == start_index:
                break
        return False

    def display(self):
        for i, entry in enumerate(self.table):
            if entry is None:
                print(f"[{i}] -> empty")
            elif entry is self.deleted:
                print(f"[{i}] -> deleted")
            else:
                print(f"[{i}] -> {entry[0]}: {entry[1]}")

    def to_dict(self):
        return {key: value for entry in self.table
                if entry is not None and entry is not self.deleted
                for key, value in [entry]}

    def from_dict(self, data):
        for key, value in data.items():
            self.insert(key, value)
