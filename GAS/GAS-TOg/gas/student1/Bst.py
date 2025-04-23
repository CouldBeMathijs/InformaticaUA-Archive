class Node:
    def __init__(self, key, value):
        """
        Geen deel van de linked chain, wel de elementen waaruit die zijn opgebouwd.
        Maakt een nieuwe node aan die nergens naar verwijst en wiens key en value meegegeven worden als parameter.
        """
        self.key = key
        self.value = value
        self.LeftChild = None
        self.RightChild = None

def createTreeItem(key, val):
    return Node(key, val)

class BST:
    def __init__(self):
        """
        Maakt een nieuwe binaire zoekboom
        precondition: de binaire zoekboom bestaat niet
        postcondition: de BST bestaat en is leeg
        """
        self.root = None

    def isEmpty(self):
        """
        Checkt of de zoekboom leeg is
        :return: (isEmpty:boolean)
        """
        return self.root is None

    def searchTreeInsert(self, newItem):
        """
        Voegt een nieuw item toe aan de zoekboom
        :param newItem: (newItem:ItemType)
        :return: (success:boolean)
        """
        if isinstance(newItem, tuple):
            newItem = Node(newItem[0], newItem[1])

        if self.isEmpty():
            self.root = newItem
            return True
        else:
            return self._insertRec(self.root, newItem)

    def _insertRec(self, current, newItem):
        if newItem.key < current.key:
            if current.LeftChild is None:
                current.LeftChild = newItem
                return True
            else:
                return self._insertRec(current.LeftChild, newItem)
        elif newItem.key > current.key:
            if current.RightChild is None:
                current.RightChild = newItem
                return True
            else:
                return self._insertRec(current.RightChild, newItem)
        return False

    def searchTreeDelete(self, searchKey):
        """
        Deletes a node from the BST given a search key.
        :param searchKey: The key to search and delete.
        :return: True if deleted, False if not found.
        """
        if self.isEmpty():
            return False
        else:
            self.root, deleted = self._deleteRec(self.root, searchKey)
            return deleted

    def _deleteRec(self, current, key):
        if current is None:
            return current, False  # Node not found

        if key < current.key:
            current.LeftChild, deleted = self._deleteRec(current.LeftChild, key)
        elif key > current.key:
            current.RightChild, deleted = self._deleteRec(current.RightChild, key)
        else:
            # Node found; delete it
            if current.LeftChild is None:
                return current.RightChild, True
            elif current.RightChild is None:
                return current.LeftChild, True

            # Node with two children: Get inorder successor
            min_larger_node = self._minValueNode(current.RightChild)
            current.key, current.value = min_larger_node.key, min_larger_node.value
            # Delete the inorder successor
            current.RightChild, _ = self._deleteRec(current.RightChild, min_larger_node.key)
            return current, True

        return current, deleted  # Ensure we return the modified tree and deletion status

    def _minValueNode(self, node):
        """
        Zoek de kleinste node in de rechtersubboom (= inorder successor)
        """
        while node.LeftChild is not None:
            node = node.LeftChild
        return node

    def inorderTraverseValue(self, visit):
        """
        Inorder traversal of the binary search tree en roep een functie op voor elke knoop in de boom.
        :param visit: (function)
        """
        self._inorderTraverseValRec(self.root, visit)

    def _inorderTraverseValRec(self, node, visit):
        if node is not None:
            self._inorderTraverseValRec(node.LeftChild, visit)
            visit(node.value)
            self._inorderTraverseValRec(node.RightChild, visit)

    def inorderTraverseKeys(self, visit):
        """
        Inorder traversal of the binary search tree en roep een functie op voor elke knoop in de boom.
        :param visit: (function)
        """
        self._inorderTraverseKeysRec(self.root, visit)

    def _inorderTraverseKeysRec(self, node, visit):
        if node is not None:
            self._inorderTraverseKeysRec(node.LeftChild, visit)
            visit(node.key)
            self._inorderTraverseKeysRec(node.RightChild, visit)

    def searchTreeRetrieve(self, searchKey):
        """
        Zoek een specifiek item in een zoekboom
        :return: (searchKey:KeyType)
        :return: (success:boolean)
        """
        return self._retrieveRec(self.root, searchKey)

    def _retrieveRec(self, current, key):
        if current is None:
            return None, False
        if key == current.key:
            return current.value, True
        elif key < current.key:
            return self._retrieveRec(current.LeftChild, key)
        else:
            return self._retrieveRec(current.RightChild, key)

    def getLength(self):
        """
        Geeft de lengte van de boom terug
        :return: (length:int)
        """
        return self._getLengthRec(self.root)

    def _getLengthRec(self, node):
        if node is None:
            return 0
        return 1 + self._getLengthRec(node.LeftChild) + self._getLengthRec(node.RightChild)


    def save(self):
        """
        Slaat een binaire zoekboom op als een dictionary
        :return: (fullSearchTree:dictionary)
        """
        # Roep de recursieve functie op vanaf de wortel
        return self._saveRec(self.root)
    def _saveRec(self, node):
        # Als de knoop leeg is, geef None terug
        if node is None:
            return None
        # Als beide kinderen niet bestaan, geef alleen de wortel terug
        if node.LeftChild is None and node.RightChild is None:
            return {'root': node.value}
        # Anders bestaan beide kinderen, roep deze functie dan op, en geef deze weer als een dictionary
        return {
            'root': node.value,
            'children': [self._saveRec(node.LeftChild), self._saveRec(node.RightChild)]
        }
    def load(self, dictionary):
        """
        Laadt een dictionary als binaire zoekboom
        :param dictionary: (fullSearchTree:dictionary)
        """
        # Als de meegegeven dictionary bestaat
        if dictionary is not None:
            # Start vanaf de wortel, en vul zo de hele BST recursief in
            self.root = self._loadRec(dictionary)

    def _loadRec(self, dictionary):
        # Als de meegegeven dictionary niet bestaat, stop
        if dictionary is None:
            return None
        # Maak een nieuwe knoop aan uit de huidige dictionary key
        node = Node(dictionary['root'], dictionary['root'])
        # Maak de kinderen aan
        left_child = None
        right_child = None
        # Kijk na of 'children' bestaat, en minstens 2 elementen heeft
        if 'children' in dictionary:
            children = dictionary['children']
            # Als er minstens 1 kind is, is dit het linkerkind, en bouw deze subtree recursief verder op
            if len(children) > 0:
                left_child = self._loadRec(children[0])
            # Als er 2 kinderen zijn, is [1] het rechterkind, en bouw dan deze rechtersubtree recursief verder op
            if len(children) > 1:
                right_child = self._loadRec(children[1])
        # Geef de kinderen aan de knoop
        node.LeftChild = left_child
        node.RightChild = right_child
        # En geef de node terug
        return node

class BSTTable(BST):
    """
    Wrapper for My BST implementation to ADT Table
    """
    def tableIsEmpty(self):
        return self.isEmpty()
    def tableLength(self):
        return self.getLength()
    def tableInsert(self, newItem):
        return self.searchTreeInsert(newItem)
    def tableDelete(self, key):
        return self.searchTreeDelete(key)
    def tableRetrieve(self, key):
        return self.searchTreeRetrieve(key)
    def traverseTableValues(self, visit):
        self.inorderTraverseValue(visit)
    def traverseTableKeys(self, visit):
        self.inorderTraverseKeys(visit)



