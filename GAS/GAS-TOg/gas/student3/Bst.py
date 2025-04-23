def createTreeItem(key, value):
    return (key, value)

class BST:
    def __init__(self, value=None):
        self.root = value
        self.left = None
        self.right = None
        self.parent = None

    def isEmpty(self):
        return self.root is None

    def searchTreeRetrieve(self, key):
        if self.root is None:
            return (None, False)
        current = self
        while current.root and current.root[0] != key:
            if key > current.root[0] and current.right is not None:
                current = current.right
            elif key < current.root[0] and current.left is not None:
                current = current.left
            else:
                return (None, False)
        return (current.root[1], True) if current.root else (None, False)

    def _RetrieveNode(self, key):
        if self.root is None:
            return None
        current = self
        while current.root and current.root[0] != key:
            if key > current.root[0] and current.right is not None:
                current = current.right
            elif key < current.root[0] and current.left is not None:
                current = current.left
            else:
                return None
        return current if current.root else None

    def searchTreeInsert(self, item):
        key, value = item
        if self.root is None:
            self.root = item
            return True
        current = self
        while current is not None:
            if key < current.root[0]:
                if current.left is None:
                    current.left = BST(item)
                    current.left.parent = current
                    return True
                current = current.left
            elif key > current.root[0]:
                if current.right is None:
                    current.right = BST(item)
                    current.right.parent = current
                    return True
                current = current.right
            else:
                return False
        return False

    def searchTreeDelete(self, key):
        node_to_delete = self._RetrieveNode(key)
        if node_to_delete is None:
            return False
        if node_to_delete.left is None and node_to_delete.right is None:
            if node_to_delete.parent is None:
                self.root = None
            elif node_to_delete == node_to_delete.parent.left:
                node_to_delete.parent.left = None
            else:
                node_to_delete.parent.right = None
            return True
        if node_to_delete.left is None or node_to_delete.right is None:
            child = node_to_delete.left if node_to_delete.left else node_to_delete.right
            if node_to_delete.parent is None:
                self.root = child.root
                self.left = child.left
                self.right = child.right
                if self.left:
                    self.left.parent = self
                if self.right:
                    self.right.parent = self
            elif node_to_delete == node_to_delete.parent.left:
                node_to_delete.parent.left = child
                child.parent = node_to_delete.parent
            else:
                node_to_delete.parent.right = child
                child.parent = node_to_delete.parent
            return True
        successor = self._get_min(node_to_delete.right)
        node_to_delete.root = successor.root
        if successor.parent.left == successor:
            successor.parent.left = successor.right
        else:
            successor.parent.right = successor.right
        if successor.right:
            successor.right.parent = successor.parent
        return True

    def _get_min(self, node):
        while node.left is not None:
            node = node.left
        return node

    def inorderTraverseKey(self, func):
        self._traverseRECKey(self, func)

    def _traverseRECKey(self, node, func):
        if node is not None:
            if node.left is not None:
                self._traverseRECKey(node.left, func)
            # Add this check to prevent potential errors
            if node.root is not None:
                func(node.root[0])
            if node.right is not None:
                self._traverseRECKey(node.right, func)

    def inorderTraverseValue(self, func):
        self._traverseRECValue(self, func)

    def _traverseRECValue(self, node, func):
        if node is not None and node.root is not None:
            if node.left is not None:
                self._traverseRECValue(node.left, func)
            func(node.root[1])
            if node.right is not None:
                self._traverseRECValue(node.right, func)

    def load(self, ingave):
        self._load(ingave)

    def _load(self, ingave):
        self.root = ingave.get('root', None)
        self.left = None
        self.right = None

        if 'children' in ingave:
            left_child, right_child = ingave['children']
            if left_child is not None:
                self.left = BST()
                self.left.load(left_child)
                self.left.parent = self
            if right_child is not None:
                self.right = BST()
                self.right.load(right_child)
                self.right.parent = self

    def save(self):
        return self._save(self)

    def _save(self, node):
        if node is None:
            return None
        result = {'root': node.root}
        children = []
        left_child = self._save(node.left)
        right_child = self._save(node.right)
        if left_child or right_child:
            children.append(left_child)
            children.append(right_child)
            result['children'] = children
        return result

    def getLength(self):
        return self._getLength(self)

    def _getLength(self, node):
        if node is None:
            return 0
        if node.root is None:  # Check if node exists but has no value
            return 0
        return 1 + self._getLength(node.left) + self._getLength(node.right)

class BSTTable(BST):
    def tableInsert(self, item):
        return self.searchTreeInsert(item)
    def tableDelete(self, key):
        return self.searchTreeDelete(key)
    def tableRetrieve(self, key):
        return self.searchTreeRetrieve(key)
    def tableIsEmpty(self):
        return self.isEmpty()
    def traverseTableKeys(self, func):
        self.inorderTraverseKey(func)
    def traverseTableValues(self, func):
        self.inorderTraverseValue(func)
    def tableLength(self):
        return self.getLength()