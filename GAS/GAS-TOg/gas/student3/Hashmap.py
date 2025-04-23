class Node:
    def __init__(self, searchkey, value):
        self.searchkey = searchkey
        self.value = value
        self.next = None


def createTableItem(searchkey, value):
    return searchkey, value


class Hashmap:

    def __init__(self):
        self.length = 999
        self.type = "quad"
        self.items = [None] * self.length

    def hash(self, input):
        return input % self.length

    def tableIsEmpty(self):
        amountempyt = 0
        for items in self.items:
            if items is None:
                amountempyt += 1
        if amountempyt == self.length:
            return True
        else:
            return False

    def isFull(self):
        amountfull = 0
        for items in self.items:
            if items is not None:
                amountfull += 1
        if amountfull == self.length:
            return True
        else:
            return False

    def tableInsert(self, item):
        index = self.hash(item[0])
        if self.type == "lin":
            i = index
            while self.items[i] is not None:
                if self.items[i][0] == item[0]:
                    return False
                i = (i + 1) % self.length
                if i == index:
                    return False
            self.items[i] = item
            return True

        elif self.type == "quad":
            if self.items[index] is None:
                self.items[index] = item
                return True
            if self.items[index][0] == item[0]:
                return False
            for j in range(1, self.length):
                i = (index + j ** 2) % self.length
                if self.items[i] is None:
                    self.items[i] = item
                    return True
                if self.items[i][0] == item[0]:
                    return False
            return False

        elif self.type == "sep":
            current = self.items[index]
            while current and current.searchkey != item[0]:
                current = current.next
            if current:
                return False

            NewNode = Node(item[0], item[1])
            NewNode.next = self.items[index]
            self.items[index] = NewNode
            return True

    def tableRetrieve(self, hashkey):
        index = self.hash(hashkey)
        if self.items[index] is None:
            return None, False
        if self.items[index][0] == hashkey:
            return self.items[index][1], True
        if self.type == "lin":
            if self.items[index][0] != hashkey:
                i = (index + 1) % self.length
                while i != index:
                    item = self.items[i]
                    if item and item[0] == hashkey:
                        return item[1], True
                    i = (i + 1) % self.length
                return None, False

        elif self.type == "quad":
            if self.items[index][0] != hashkey:
                for j in range(1, self.length):
                    i = (index + j ** 2) % self.length
                    if self.items[i] and self.items[i][0] == hashkey:
                        return self.items[i][1], True
                return None, False

        elif self.type == "sep":
            current = self.items[index]
            while current is not None:
                key_match = (current.searchkey == hashkey)
                if key_match:
                    return current.value, True
                current = current.next
            return None, False

    def tableDelete(self, hashkey):
        index = self.hash(hashkey)
        current = self.items[index]

        if current is None:
            return False

        if self.type == "lin":
            return self._linearDelete(index, hashkey)

        if self.type == "quad":
            return self._quadraticDelete(index, hashkey)

        if self.type == "sep":
            return self._separateChainingDelete(index, hashkey)

    def _linearDelete(self, index, hashkey):
        i = index
        while self.items[i] is not None:
            if self.items[i][0] == hashkey:
                self.items[i] = None
                return True
            i = (i + 1) % self.length
            if i == index:
                break
        return False

    def _quadraticDelete(self, index, hashkey):
        i, j = index, 1
        while self.items[i] is not None:
            if self.items[i][0] == hashkey:
                self.items[i] = None
                return True
            i = (index + j ** 2) % self.length
            j += 1
            if i == index:
                break
        return False

    def _separateChainingDelete(self, index, hashkey):
        current = self.items[index]
        if current.searchkey == hashkey:
            self.items[index] = current.next
            return True

        prev = current
        current = current.next
        while current:
            if current.searchkey == hashkey:
                prev.next = current.next
                return True
            prev = current
            current = current.next
        return False

    def tableLength(self):
        count = 0
        for i in self.items:
            if i is not None:
                count += 1
        return count

    def traverseTable(self, func):
        for i in self.items:
            if i is not None:
                func(i)

    def save(self):
        return {"type": self.type, "items": self._saveItems()}

    def _saveItems(self):
        if self.type == "sep":
            return [self._saveSeparateChaining(item) for item in self.items]
        else:
            return [item[1] if item else None for item in self.items]

    def _saveSeparateChaining(self, item):
        if item is None:
            return None
        values = []
        current = item
        while current:
            values.append(current.value)
            current = current.next
        return values

    def load(self, input):
        self.type = input["type"]
        self.items = [self._loadItem(item) for item in input["items"]]
        self.length = len(input["items"])

    def _loadItem(self, item):
        if item is None:
            return None
        if self.type == "sep":
            return self._loadSeparateChaining(item)
        else:
            return createTableItem(item, item)

    def _loadSeparateChaining(self, values):
        head = None
        for val in reversed(values):
            new_node = Node(val, val)
            new_node.next = head
            head = new_node
        return head


class HashmapTable(Hashmap):
    def tableInsert(self, value):
        return Hashmap.tableInsert(self, value)

    def tableDelete(self, value):
        return Hashmap.tableDelete(self, value)

    def tableRetrieve(self, hashkey):
        return Hashmap.tableRetrieve(self, hashkey)

    def tableIsEmpty(self):
        return Hashmap.tableIsEmpty(self)

    def tableLength(self):
        return Hashmap.tableLength(self)

    def traverseTable(self, func):
        Hashmap.traverseTable(self, func)

    def traverseTableKeys(self, func):
        for i in self.items:
            if i is not None:
                if self.type == "sep":
                    current = i
                    while current:
                        func(current.searchkey)
                        current = current.next
                else:
                    func(i[0])

    def traverseTableValues(self, func):
        for i in self.items:
            if i is not None:
                if self.type == "sep":
                    current = i
                    while current:
                        func(current.value)
                        current = current.next
                else:
                    func(i[1])