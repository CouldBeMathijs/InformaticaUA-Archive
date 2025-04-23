def createTreeItem(k, v):
    """
    Precondition(s):
      - k is a comparable type (so it can be sorted).
      - v can be any type or object.

    Postcondition(s):
      - Returns a simple dictionary with "key" set to k and "value" set to v.
    """
    return {"key": k, "value": v}


class TwoThreeTreeNode:
    """
    This class represents a node within a 2-3 tree structure. It can hold up to two items 
    and manages up to three children.

    Precondition(s):
      - items (if given) must be a list of dicts, each having "key" and "value" fields.
      - children (if given) must be a list of valid TwoThreeTreeNode objects, up to three of them.

    Postcondition(s):
      - Creates a node with those items, defaulting to an empty list if none are provided.
      - Sets its children, also defaulting to an empty list if none are given.
    """

    def __init__(self, items=None, children=None):
        # If the caller doesn't provide items, we just initialize to an empty list.
        self.items = items if items else []
        self.children = children if children else []

    def isLeaf(self):
        """
        Precondition(s):
          - Always safe to call, no special requirements.

        Postcondition(s):
          - Returns True if there are no child nodes; False otherwise.
        """
        return len(self.children) == 0

    def insertItem(self, newItem):
        """
        Insert an item (dict with "key" and "value") into the node, keeping items sorted by key.

        Precondition(s):
          - newItem has 'key' (comparable) and 'value'.

        Postcondition(s):
          - The node's items remain in ascending order by key.
        """
        
        self.items.append(newItem)
        self.items.sort(key=lambda x: x["key"])

    def removeItem(self, key):
        """
        Remove the item with a matching 'key', if it exists in this node.

        Precondition(s):
          - key is used to find a matching dictionary in self.items.

        Postcondition(s):
          - If found, the item is deleted from self.items; otherwise nothing changes.
        """
        self.items = [itm for itm in self.items if itm["key"] != key]


class TwoThreeTree:
    """
    A standard 2-3 tree with insert, delete, retrieve, and traversal methods.

    Precondition(s):
      - No special initialization parameters are required.

    Postcondition(s):
      - A valid (possibly empty) 2-3 tree is created.
    """

    def __init__(self):
        # Start with an empty tree.
        self.root = None

    def isEmpty(self):
        """
        Precondition(s):
          - None.

        Postcondition(s):
          - Returns True if the tree has no root node; otherwise False.
        """
        return self.root is None

    def insertItem(self, item):
        """
        Insert a (key, value) dictionary into the 2-3 tree.

        Precondition(s):
          - 'item' must contain a 'key' that supports comparison and a 'value' field.

        Postcondition(s):
          - The item is placed in the correct position; any node exceeding 2 items gets split.
          - Returns True if insertion completes; returns False if an unexpected error occurs.
        """
        _, found = self.retrieveItem(item["key"])
        if found:
          return False

        try:
            if not self.root:
                self.root = TwoThreeTreeNode(items=[item])
            else:
                maybe_new_root = self._insert(self.root, item)
                if maybe_new_root:  # If the root was split
                    self.root = maybe_new_root
            return True
        except:
            return False

    def _insert(self, node, item):
        """
        Internal helper for insertion. Recursively finds the correct spot for 'item'.

        Precondition(s):
          - node is a valid TwoThreeTreeNode.
          - item has 'key' which supports < comparisons.

        Postcondition(s):
          - The item is inserted into the correct leaf; splits the node if it ends up with 3 items.
          - Returns a new node if a split occurs; None if no splitting happens.
        """
        if node.isLeaf():
            node.insertItem(item)
            if len(node.items) > 2:
                return self._split(node)
            return None

        # Decide which child to follow based on the key.
        if item["key"] < node.items[0]["key"]:
            idx = 0
        elif len(node.items) == 1 or item["key"] < node.items[1]["key"]:
            idx = 1
        else:
            idx = 2

        # Ensure the child list is large enough.
        if idx >= len(node.children):
            node.children.extend([None] * (idx - len(node.children) + 1))

        if not node.children[idx]:
            node.children[idx] = TwoThreeTreeNode()

        new_child = self._insert(node.children[idx], item)
        if new_child:
            # Insert the middle item of new_child into the current node.
            node.items.insert(idx, new_child.items[0])
            node.children[idx] = new_child.children[0]
            node.children.insert(idx + 1, new_child.children[1])

            if len(node.items) > 2:
                return self._split(node)

        return None

    def _split(self, node):
        """
        If a node has 3 items, split it into two separate nodes and return a new parent with the middle item.

        Precondition(s):
          - node has exactly 3 items in node.items.

        Postcondition(s):
          - Returns a freshly created node containing the middle item. 
          - The original node's items/children are divided between the two children.
        """
        mid_item = node.items[1]
        left_side = TwoThreeTreeNode(items=[node.items[0]], children=node.children[:2])
        right_side = TwoThreeTreeNode(items=[node.items[2]], children=node.children[2:])
        return TwoThreeTreeNode(items=[mid_item], children=[left_side, right_side])

    def deleteItem(self, key):
        """
        Remove the item with the given key from the 2-3 tree if it exists.

        Precondition(s):
          - key supports comparison with stored keys.

        Postcondition(s):
          - If the key is found, the item is removed and the tree is rebalanced if needed.
          - Returns True unless there's an unexpected error (like IndexError), in which case False.
        """
        if not self.root:
            return False

        _, found = self.retrieveItem(key)
        if not found:
            return False

        try:
            self._delete(self.root, key)
            # If the root loses all items, shrink it.
            if self.root and len(self.root.items) == 0:
                self.root = self.root.children[0] if self.root.children else None
            return True
        except IndexError:
            return False

    def _delete(self, node, key):
        """
        Recursive helper for deletion.

        Precondition(s):
          - node is a valid TwoThreeTreeNode.
          - key is the key to remove.

        Postcondition(s):
          - Key is removed from this subtree if found; merges/balances as needed.
          - Returns True if the node ends up with no items; otherwise False.
        """
        # Check if the current node holds the key.
        to_remove_idx = next((i for i, itm in enumerate(node.items) if itm["key"] == key), -1)

        if to_remove_idx != -1:
            # Found it in the current node
            if node.isLeaf():
                node.items.pop(to_remove_idx)
                return len(node.items) == 0
            else:
                # If not a leaf, replace with inorder successor
                successor_child_idx = to_remove_idx + 1
                successor_item = self._get_inorder_successor(node.children[successor_child_idx])
                node.items[to_remove_idx] = successor_item
                empty_child = self._delete(node.children[successor_child_idx], successor_item["key"])
                if empty_child:
                    self._rebalance(node, successor_child_idx)
        else:
            # Key not here, keep descending
            child_idx = self._get_child_index(node, key)
            if child_idx >= len(node.children) or node.children[child_idx] is None:
                return False
            empty_child = self._delete(node.children[child_idx], key)
            if empty_child:
                self._rebalance(node, child_idx)

        if node is self.root and len(node.items) == 0:
            self.root = node.children[0] if node.children else None

        return len(node.items) == 0

    def _get_child_index(self, node, key):
        """
        Decide which child branch to follow based on the given key.

        Precondition(s):
          - node has 1 or 2 items.

        Postcondition(s):
          - Returns the child index (0, 1, or 2) to explore next.
        """
        if key < node.items[0]["key"]:
            return 0
        elif len(node.items) == 1 or key < node.items[1]["key"]:
            return 1
        else:
            return 2

    def _get_inorder_successor(self, node):
        """
        Finds the leftmost item in the given subtree.

        Precondition(s):
          - node is not None.

        Postcondition(s):
          - Returns the first (smallest) item in the subtree.
        """
        while not node.isLeaf():
            node = node.children[0]
        return node.items[0]

    def _rebalance(self, parent_node, child_idx):
        """
        Correct an underflow situation in parent_node.children[child_idx].

        Precondition(s):
          - parent_node must have at least 1 item.
          - child_idx is in range for parent_node.children.

        Postcondition(s):
          - The child's deficiency is resolved by borrowing from siblings or merging.
        """
        left_avail = (child_idx > 0 and len(parent_node.children[child_idx - 1].items) > 1)
        right_avail = (
            child_idx < len(parent_node.children) - 1 and 
            len(parent_node.children[child_idx + 1].items) > 1
        )

        if left_avail:
            self._borrow_from_left(parent_node, child_idx)
        elif right_avail:
            self._borrow_from_right(parent_node, child_idx)
        else:
            # Merge with either left or right sibling
            if child_idx > 0:
                self._merge(parent_node, child_idx - 1)
            else:
                self._merge(parent_node, child_idx)

    def _borrow_from_left(self, parent, c_idx):
        """
        Borrow an item from the left sibling if it has more than one.

        Precondition(s):
          - The left sibling has at least 2 items.

        Postcondition(s):
          - One item moves from the left sibling to the child through the parent.
        """
        left_sib = parent.children[c_idx - 1]
        child_node = parent.children[c_idx]

        child_node.items.insert(0, parent.items[c_idx - 1])
        parent.items[c_idx - 1] = left_sib.items.pop()

        if left_sib.children:
            child_node.children.insert(0, left_sib.children.pop())

    def _borrow_from_right(self, parent, c_idx):
        """
        Borrow an item from the right sibling if it has more than one.

        Precondition(s):
          - The right sibling has at least 2 items.

        Postcondition(s):
          - An item is moved from the right sibling to the child via the parent.
        """
        right_sib = parent.children[c_idx + 1]
        child_node = parent.children[c_idx]

        child_node.items.append(parent.items[c_idx])
        parent.items[c_idx] = right_sib.items.pop(0)

        if right_sib.children:
            child_node.children.append(right_sib.children.pop(0))

    def _merge(self, parent, idx):
        """
        Merge two sibling nodes at indices idx and idx+1 in the parent's children array.

        Precondition(s):
          - parent has at least one item.
          - idx+1 < len(parent.children).

        Postcondition(s):
          - The items of children[idx+1] are merged into children[idx].
          - The parent's item at position idx is added to that merged child.
          - The sibling child is removed from parent.children.
        """
        child_node = parent.children[idx]
        sibling_node = parent.children[idx + 1]

        child_node.items.append(parent.items.pop(idx))
        child_node.items.extend(sibling_node.items)

        if sibling_node.children:
            child_node.children.extend(sibling_node.children)

        parent.children.pop(idx + 1)

    def retrieveItem(self, key):
        """
        Look up a value by key.

        Precondition(s):
          - key must be comparable to stored keys.

        Postcondition(s):
          - Returns (found_value, True) if found; otherwise (None, False).
        """
        found = self._retrieve(self.root, key)
        return (found["value"], True) if found else (None, False)

    def _retrieve(self, node, key):
        """
        Internal helper for retrieval.

        Precondition(s):
          - node can be None or a valid TwoThreeTreeNode.
          - key is comparable to node.items' keys.

        Postcondition(s):
          - If an item with 'key' is found, return that dictionary; else return None.
        """
        if not node:
            return None

        for elem in node.items:
            if elem["key"] == key:
                return elem

        index = self._get_child_index(node, key)
        if index >= len(node.children) or not node.children[index]:
            return None

        return self._retrieve(node.children[index], key)

    def inorderTraverseKey(self, func):
        """
        Traverse the tree in ascending order of keys, applying 'func' to each value encountered.

        Precondition(s):
          - func is callable with one argument.

        Postcondition(s):
          - Each value in the tree is passed to func in ascending order.
        """
        self._inorderKey(self.root, func)

    def _inorderKey(self, node, func):
        """
        Internal helper for inorder traversal.

        Precondition(s):
          - node can be None or a valid TwoThreeTreeNode.
          - func is callable.

        Postcondition(s):
          - Visits items in sorted key order, calling func(value) for each.
        """
        if not node:
            return

        if node.isLeaf():
            for item_data in node.items:
                func(item_data["key"])
        else:
            for i in range(len(node.items)):
                self._inorderKey(node.children[i], func)
                func(node.items[i]["key"])
            self._inorderKey(node.children[-1], func)

    def inorderTraverseValue(self, func):
        """
        Traverse the tree in ascending order of keys, applying 'func' to each value encountered.

        Precondition(s):
          - func is callable with one argument.

        Postcondition(s):
          - Each value in the tree is passed to func in ascending order.
        """
        self._inorderValue(self.root, func)

    def _inorderValue(self, node, func):
        """
        Internal helper for inorder traversal.

        Precondition(s):
          - node can be None or a valid TwoThreeTreeNode.
          - func is callable.

        Postcondition(s):
          - Visits items in sorted key order, calling func(value) for each.
        """
        if not node:
            return

        if node.isLeaf():
            for item_data in node.items:
                func(item_data["value"])
        else:
            for i in range(len(node.items)):
                self._inorderValue(node.children[i], func)
                func(node.items[i]["value"])
            self._inorderValue(node.children[-1], func)

    def save(self):
        """
        Serialize the entire tree into a nested dictionary format.

        Precondition(s):
          - None.

        Postcondition(s):
          - Returns a dict describing the tree's structure, or None if empty.
        """
        return self._save(self.root)

    def _save(self, node):
        """
        Recursive helper for saving the tree structure.

        Precondition(s):
          - node can be None or a valid TwoThreeTreeNode.

        Postcondition(s):
          - Returns a dict with "root": [list of keys] and possibly "children" if not a leaf.
          - Returns None if node is None.
        """
        if not node:
            return None

        result = {"root": [itm["key"] for itm in node.items]}
        if not node.isLeaf():
            result["children"] = [self._save(ch) for ch in node.children]
        return result

    def load(self, data):
        """
        Rebuild the tree from a dictionary (which should match the format generated by save()).

        Precondition(s):
          - data must be a dict or None, consistent with the structure returned by save().

        Postcondition(s):
          - Replaces this tree with the structure described in 'data'.
        """
        self.root = self._load(data)

    def _load(self, data):
        """
        Internal helper for loading a serialized tree.

        Precondition(s):
          - data can be None or a dict with "root" and optional "children".

        Postcondition(s):
          - Returns a newly built TwoThreeTreeNode (or None).
        """
        if not data:
            return None

        node = TwoThreeTreeNode(items=[createTreeItem(k, k) for k in data["root"]])
        if "children" in data:
            node.children = [self._load(child_data) for child_data in data["children"]]
        return node
    
    def getLength(self):
        """
        Get the number of items in the tree.

        Precondition(s):
          - None.

        Postcondition(s):
          - Returns the total number of items stored in the tree.
        """
        return self._getLength(self.root)
    
    def _getLength(self, node):
        """
        Internal helper for counting items in the tree.

        Precondition(s):
          - node can be None or a valid TwoThreeTreeNode.

        Postcondition(s):
          - Returns the total number of items stored in the subtree.
        """
        if not node:
            return 0
        return len(node.items) + sum(self._getLength(ch) for ch in node.children)
    
class TwoThreeTreeTable(TwoThreeTree):
    """
    wrapper for two three tree
    """
    def tableIsEmpty(self):
        return self.isEmpty()

    def tableLength(self):
        return self.getLength()

    def tableInsert(self, item):
        item = createTreeItem(item[0], item[1])
        return self.insertItem(item)

    def tableDelete(self, key):
        return self.deleteItem(key)

    def tableRetrieve(self, key):
        return self.retrieveItem(key)

    def traverseTableValues(self, visit):
        self.inorderTraverseValue(visit)

    def traverseTableKeys(self, visit):
        self.inorderTraverseKey(visit)