def createTreeItem(key, val):
    """Create a TreeItem with a key and a value."""
    return {"key": key, "val": val}


class Node:
    """A node in the binary search tree."""
    def __init__(self, data):
        self.data = data  # data is a dictionary with 'key' and 'val'
        self.left = None
        self.right = None


class BST:
    """Binary Search Tree (BST) implementation."""
    def __init__(self):
        self.root = None

    def isEmpty(self):
        """Check if the tree is empty."""
        return self.root is None

    def searchTreeInsert(self, item):
        """Insert a new TreeItem into the BST."""
        if self.isEmpty():
            self.root = Node(item)
            return True
        else:
            return self._insert(self.root, item)

    def _insert(self, node, item):
        if item["key"] < node.data["key"]:
            if node.left is None:
                node.left = Node(item)
                return True
            else:
                return self._insert(node.left, item)
        elif item["key"] > node.data["key"]:
            if node.right is None:
                node.right = Node(item)
                return True
            else:
                return self._insert(node.right, item)
        return False  # Duplicate keys are not allowed

    def searchTreeRetrieve(self, key):
        """Retrieve a TreeItem by its key."""
        result = self._retrieve(self.root, key)
        if result:
            return (result["val"], True)
        return (None, False)

    def _retrieve(self, node, key):
        if node is None:
            return None
        if key == node.data["key"]:
            return node.data
        elif key < node.data["key"]:
            return self._retrieve(node.left, key)
        else:
            return self._retrieve(node.right, key)

    def searchTreeDelete(self, key):
        """Delete a TreeItem by its key."""
        deleted, self.root = self._delete(self.root, key)
        return deleted

    def _delete(self, node, key):
        if node is None:
            return False, None

        if key < node.data["key"]:
            deleted, node.left = self._delete(node.left, key)
            return deleted, node
        elif key > node.data["key"]:
            deleted, node.right = self._delete(node.right, key)
            return deleted, node
        else:
            # Node with the key found
            if node.left is None:
                return True, node.right
            elif node.right is None:
                return True, node.left

            # Node with two children: replace with inorder successor
            successor = self._minValueNode(node.right)
            node.data = successor.data
            deleted, node.right = self._delete(node.right, successor.data["key"])
            return True, node

    def _minValueNode(self, node):
        """Find the node with the minimum key."""
        while node.left is not None:
            node = node.left
        return node

    def inorderTraverseKey(self, func):
        """Traverse the BST in-order and apply a function."""
        self._inorderKey(self.root, func)

    def _inorderKey(self, node, func):
        if node is not None:
            self._inorderKey(node.left, func)
            func(node.data["key"])
            self._inorderKey(node.right, func)

    def inorderTraverseValue(self, func):
        """Traverse the BST in-order and apply a function."""
        self._inorderValue(self.root, func)

    def _inorderValue(self, node, func):
        if node is not None:
            self._inorderValue(node.left, func)
            func(node.data["val"])
            self._inorderValue(node.right, func)

    def save(self):
        """Export the tree to a nested dictionary."""
        return self._save(self.root)

    def _save(self, node):
        if node is None:
            return None
        
        result = {"root": node.data["key"]}

        children = [
            self._save(node.left),
            self._save(node.right)
        ]

        # Remove empty children ([None, None])
        if any(children):
            result["children"] = children

        return result


    def load(self, data):
        """Load a tree from a nested dictionary."""
        self.root = self._load(data)

    def _load(self, data):
        if data is None:
            return None
        node = Node(createTreeItem(data["root"], data["root"]))
        if "children" in data:
            node.left = self._load(data["children"][0])
            node.right = self._load(data["children"][1])
        return node
    
    def getLength(self):
        return self._getLength(self.root)
    
    def _getLength(self, node):
        if node is None:
            return 0
        return 1 + self._getLength(node.left) + self._getLength(node.right)
    

class BSTTable(BST):
    """
    Wrapper for my bst
    """
    def tableIsEmpty(self):
        return self.isEmpty()
    
    def tableLength(self):
        return self.getLength()
    
    def tableInsert(self, item):
        item = createTreeItem(item[0], item[1])
        return self.searchTreeInsert(item)
    
    def tableDelete(self, key):
        return self.searchTreeDelete(key)
    
    def tableRetrieve(self, key):
        return self.searchTreeRetrieve(key)
    
    def traverseTableValues(self, func):
        self.inorderTraverseValue(func)

    def traverseTableKeys(self, func):
        self.inorderTraverseKey(func)