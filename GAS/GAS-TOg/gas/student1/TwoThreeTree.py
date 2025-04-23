class Node:
    """
    A Node in the 2-3 tree
    """
    def __init__(self, key1, kv1, key2=None, kv2=None):
        """
        Create a new Node:
        required fields: key1 (keyItemType), kv1 (valueItemType)
        Optional fields: key2 (keyItemType), kv2 (valueItemType)
        """
        self.kv1 = (key1, kv1)
        self.kv2 = (key2, kv2) if key2 is not None else (None, None)  # Ensure kv2 is always a tuple
        self.LeftChild = None
        self.RightChild = None
        self.MiddleChild = None
        self.parent = None
    def amountOfChildren(self):
        """
        Returns the amount of children a node has
        """
        amount = 0
        if self.LeftChild is not None:
            amount += 1
        if self.MiddleChild is not None:
            amount += 1
        if self.RightChild is not None:
            amount += 1
        return amount
    def orphan(self):
        """
        Decouples the node from their parent
        """
        if self.parent.LeftChild == self:
            self.parent.LeftChild = None
        elif self.parent.MiddleChild == self:
            self.parent.MiddleChild = None
        elif self.parent.RightChild == self:
            self.parent.RightChild = None
        self.parent = None
        return
    def isLeftChild(self):
        if not self.parent:
            return False
        return self == self.parent.LeftChild
    def isMiddleChild(self):
        if not self.parent:
            return False
        return self == self.parent.MiddleChild
    def isRightChild(self):
        if not self.parent:
            return False
        return self == self.parent.RightChild
    def amountOfValues(self):
        """
        Returns the amount of values a node has
        """
        valnum = 0
        if self.kv1 != (None, None):
            valnum += 1
        if self.kv2 != (None, None):
            valnum += 1
        return valnum
    def addValue(self, key, value):
        """
        Adds a value to the node, ensuring the values remain sorted.
        """
        if self.kv1 == (None, None):
            self.kv1 = (key, value)
        elif self.kv2 == (None, None):
            # Place the key in the correct slot
            if key < self.kv1[0]:
                self.kv2 = self.kv1
                self.kv1 = (key, value)
            elif key > self.kv1[0]:
                self.kv2 = (key, value)
        else:
            raise ValueError("Cannot add value to a node with both slots filled")
        # Ensure values are sorted after the addition
        self.fixValues()
    def removeValue(self, key):
        """
        Removes a value from a node, ensuring the values remain sorted.
        """
        if self.kv1[0] == key or self.kv1 == key:
            self.kv1 = self.kv2
            self.kv2 = (None, None)  # Reset kv2 to (None, None)
            return True
        if self.kv2[0] == key or self.kv2 == key:
            self.kv2 = (None, None)  # Reset kv2 to (None, None)
            return True
        return False
    def swapValue(self, toDelete, toAdd):
        """
        Swaps 2 values in a node, ensuring the values remain sorted.
        """
        if toDelete is tuple:
            toDelete = toDelete[0]
        success = False
        if self.kv1 == toDelete:
            self.kv1 = toAdd
            success = True
        if self.kv2 == toDelete:
            self.kv2 = toAdd
            success = True
        if self.kv2 == (None, None):
            return success
        if self.kv1[0] > self.kv2[0]:
            temp = self.kv1
            self.kv1 = self.kv2
            self.kv2 = temp
        return success
    def fixValues(self):
        """
        Make sure the values in a Node are sorted
        """
        if self.kv2 != (None, None) and self.kv1 == (None, None):
            self.kv1 = self.kv2
            self.kv2 = (None, None)
        if self.kv2 != (None, None) and self.kv1[0] > self.kv2[0]:
            temp = self.kv1
            self.kv1 = self.kv2
            self.kv2 = temp
    def fixChildren(self):
        """
        Tries to fix the children of a node, which might be messed up after the actions of another function.
        """
        # Store all children in an array
        children = [self.LeftChild, self.MiddleChild, self.RightChild]
        # Ensure None is not in the array
        children = [child for child in children if child is not None]
        # When there are no children, there's nothing to fix
        if len(children) == 0:
            return
        # If it's a 2 Node, the children should be the left and right child and sorted by value
        if len(children) == 2 and self.amountOfValues() == 1:
            self.MiddleChild = None
            for child in children:
                if child.kv1 != (None, None):
                    if child.kv1[0] < self.kv1[0]:
                        self.LeftChild = child
                    if child.kv1[0] > self.kv1[0]:
                        self.RightChild = child
            if self.LeftChild.amountOfValues() == 2 and self.RightChild.amountOfValues() == 1 and self.LeftChild.kv2[0] > self.LeftChild.kv1[0]:
                highest = self.LeftChild.removeHighest()
                self.RightChild.addValue(highest[0], highest[1])
            return
        # If the redistribution caused 3 children to be put on a 2-Node, fix this
        if len(children) == 3 and self.amountOfValues() == 1:
            if self.LeftChild.amountOfValues() == 1 and self.MiddleChild.amountOfValues() == 1 and self.LeftChild.kv1[0] < self.kv1[0] and self.MiddleChild.kv1[0] < self.kv1[0]:
                self.LeftChild.mergeNodes(self.MiddleChild)
            if self.MiddleChild.amountOfValues() == 1 and self.RightChild.amountOfValues() == 1 and self.MiddleChild.kv1[0] > self.kv1[0] and self.RightChild.kv1[0] > self.kv1[0]:
                self.MiddleChild.mergeNodes(self.RightChild)
        if len(children) == 3 and self.amountOfValues() == 2:
            for i, child in enumerate(children):
                if child.kv1 != (None, None):
                    # If the child has a value smaller than self.kv1, it should be on the left
                    if child.kv1[0] < self.kv1[0]:
                        if self.LeftChild and self.LeftChild.kv1[0] < self.kv1[0] and self.LeftChild.kv2 == (None, None):
                            # Call mergeNodes if the left child can absorb the current child
                            self.LeftChild.mergeNodes(child)
                        else:
                            # Otherwise, assign it directly to the LeftChild
                            self.LeftChild = child
                    # If the child value is between self.kv1 and self.kv2, it should be in the middle
                    elif self.kv1[0] < child.kv1[0] < self.kv2[0]:
                        self.MiddleChild = child
                    # If the child's value is greater than self.kv2, it should go to the right
                    elif child.kv1[0] > self.kv2[0]:
                        self.RightChild = child
    
    def mergeNodes(self, sibling):
        """
        Merges the current node with a sibling. This method ensures that the
        current node absorbs values and children from the sibling.
        """
        # If the current node has no children, absorb the sibling into it
        if self == sibling:
            return
        if self.amountOfChildren() < sibling.amountOfChildren():
            self._absorb(sibling)
    
        # If the sibling has no children, absorb the current node into it
        else:
            sibling._absorb(self)
    def _absorb(self, sibling):
        """
        Absorbs values and children from the sibling into the current node.
        The sibling is orphaned after the absorption.
        """
    
        # First, absorb the values from the sibling into the current node.
        self.addValue(sibling.kv1[0], sibling.kv1[1])
        if sibling.kv2 and not self.kv2:  # If the sibling has a second value, add it too.
            self.addValue(sibling.kv2[0], sibling.kv2[1])
    
        # Now, absorb the children from the sibling (if any).
        if sibling.LeftChild:
            self._addChild(sibling.LeftChild)
        if sibling.MiddleChild:
            self._addChild(sibling.MiddleChild)
        if sibling.RightChild:
            self._addChild(sibling.RightChild)
    
        # After absorbing, the sibling is orphaned
        sibling.orphan()

    def _addChild(self, child):
        """
        Adds a child to the current node if there's space.
        This method ensures the tree structure is maintained after absorbing children.
        """
        if self.LeftChild is None:
            self.LeftChild = child
        elif self.MiddleChild is None:
            self.MiddleChild = child
        elif self.RightChild is None:
            self.RightChild = child
    
        # Ensure the child's parent is set to the current node.
        if child:
            child.parent = self
    
    def removeChild(self, child):
        """
        Removes a child from the current node's children list.
        """
        if self.LeftChild == child:
            self.LeftChild = None
        elif self.MiddleChild == child:
            self.MiddleChild = None
        elif self.RightChild == child:
            self.RightChild = None

    def siblings(self):
        """
        Returns a list of every sibling of the current Node
        """
        if self.parent is None:
            return []  # The root has no siblings
        return [child for child in [self.parent.LeftChild, self.parent.MiddleChild, self.parent.RightChild] if
                child and child != self]
    
    def redistribute(self):
        """
        Redistributes the empty leaf (self) with its parent and siblings.
        Handles cases with 2-node and 3-node parents, and all sibling configurations.
        Returns True if redistribution succeeds, False if not possible.
        """
        if self.parent is None:
            return False  # Root cannot redistribute
        if self.amountOfValues() > 0:
            return False
        parent = self.parent
        siblings = self.siblings()

        # If it's a Middle or Right child, make sure the siblings are in the right order
        if not self.isLeftChild():
            siblings = siblings[::-1]
        parent.fixChildren()
        for sibling in siblings:
            if sibling.amountOfValues() > 1:  # Sibling has excess values
                if sibling.isLeftChild():
                    if self.isMiddleChild():
                        # Take parent's kv1, give sibling's highest to parent
                        self.addValue(parent.kv1[0], parent.kv1[1])
                        # if parent.parent is None:
                        #     parent.removeValue(parent.kv1)
                        # else:
                        parent.swapValue(parent.kv1, sibling.removeHighest())
                    elif self.isRightChild():
                        # Take parent's kv2, give sibling's highest to parent
                        self.addValue(parent.kv2[0], parent.kv2[1])
                        if parent.parent is None:
                            parent.removeValue(parent.kv1)
                        else:
                            parent.swapValue(parent.kv2, sibling.removeHighest())
                elif sibling.isMiddleChild():
                    if self.isLeftChild():
                        # Take parent's kv1, give sibling's lowest to parent
                        self.addValue(parent.kv1[0], parent.kv[1])
                        if parent.parent is None:
                            parent.removeValue(parent.kv1)
                        else:
                            parent.swapValue(parent.kv1, sibling.removeLowest())
                    elif self.isRightChild():
                        # Take parent's kv2, give sibling's highest to parent
                        self.addValue(parent.kv2[0], parent.kv[1])
                        if parent.parent is None:
                            parent.removeValue(parent.kv1)
                        else:
                            parent.swapValue(parent.kv2, sibling.removeHighest())
                elif sibling.isRightChild():
                    if self.isMiddleChild():
                        # Take parent's kv2, give sibling's lowest to parent
                        self.addValue(parent.kv2[0], parent.kv2[1])
                        if parent.parent is None:
                            pass
                            #parent.removeValue(parent.kv1)
                        else:
                            pass
                        parent.swapValue(parent.kv2, sibling.removeLowest())
                    elif self.isLeftChild():
                        # Take parent's kv1, give sibling's lowest to parent
                        self.addValue(parent.kv1[0], parent.kv1[1])
                        if parent.parent is None:
                            pass
                            #parent.removeValue(parent.kv1)
                        else:
                            pass
                        parent.swapValue(parent.kv1, sibling.removeLowest())
                return True
        parent.fixChildren()
        # If no sibling can redistribute, return False
        return False

    def removeLowest(self):
        """
        Delete the lowest value in a Node, and return it
        """
        returnval = self.kv1
        self.removeValue(self.kv1[0])
        return returnval

    def removeHighest(self):
        """
        Delete the highest value in a Node and return it
        """
        if self.kv2 != (None, None):
            returnval = self.kv2
            self.removeValue(self.kv2)
        else:
            returnval = self.kv1
            self.removeValue(self.kv1)
        return returnval

    def merge(self):
        """
        Merges the current node (self) with one of its siblings and the parent.
        The merging process absorbs one value from the parent and all values from the sibling.
        Returns True if the merge is successful, otherwise False.
        """
        if self.parent is None:
            return False  # Cannot merge the root node

        parent = self.parent
        siblings = self.siblings()

        # Try to find a sibling to merge with
        for sibling in siblings:
            if sibling.amountOfValues() > 0:
                # Determine the parent's value to merge
                if self.isLeftChild():
                    parent_value = parent.kv1
                elif self.isMiddleChild():
                    parent_value = parent.kv1 if sibling.isLeftChild() else parent.kv2
                elif self.isRightChild():
                    if parent.kv2 != (None, None):
                        parent_value = parent.kv2
                    else:
                        parent_value = parent.kv1
                else:
                    return False

                # Merge parent's value and sibling's values into self
                self.addValue(parent_value[0], parent_value[1])
                # Todo: fix this
                if sibling.amountOfValues() + self.amountOfValues() > 2:
                    return False
                if sibling.kv1 != (None, None):
                    self.addValue(sibling.kv1[0], sibling.kv1[1])
                if sibling.kv2 != (None, None):
                    self.addValue(sibling.kv2[0], sibling.kv2[1])

                # Absorb sibling's children if they exist
                if sibling.LeftChild:
                    if not self.LeftChild:
                        self.LeftChild = sibling.LeftChild
                    elif not self.MiddleChild:
                        self.MiddleChild = sibling.LeftChild
                    elif not self.RightChild:
                        self.RightChild = sibling.LeftChild
                    if sibling.LeftChild:
                        sibling.LeftChild.parent = self

                if sibling.MiddleChild:
                    if not self.LeftChild:
                        self.LeftChild = sibling.MiddleChild
                    elif not self.MiddleChild:
                        self.MiddleChild = sibling.MiddleChild
                    elif not self.RightChild:
                        self.RightChild = sibling.MiddleChild
                    if sibling.MiddleChild:
                        sibling.MiddleChild.parent = self

                if sibling.RightChild:
                    if not self.LeftChild:
                        self.LeftChild = sibling.RightChild
                    elif not self.MiddleChild:
                        self.MiddleChild = sibling.RightChild
                    elif not self.RightChild:
                        self.RightChild = sibling.RightChild
                    if sibling.RightChild:
                        sibling.RightChild.parent = self

                # Update parent and remove merged sibling
                sibling.orphan()  # Disconnect sibling from the parent
                parent.removeValue(parent_value[0])
                parent.fixChildren()
                self.fixChildren()

                # If parent underflows, handle recursively
                if parent and parent.amountOfValues() == 0:
                    parent.merge()
                return True


def createTreeItem(key, val):
    return (key, val)

class TwoThreeTree:
    def __init__(self):
        """
        Initializes a new, empty 2-3 tree.
        precondition: None
        postcondition: A new tree is created with no elements (root is None).
        """
        self.root = None
    def isEmpty(self):
        """
        Checks if the tree is empty.
        precondition: The tree exists.
        postcondition: No changes made to the tree.
        :return: True if the tree is empty, False otherwise.
        """
        return self.root is None
    def insertItem(self, newItem):
        """
        Adds a new item to the 2-3 tree.
        precondition: the tree exists
        postcondition: the tree exists and contains one more element
        :return: (success:boolean)
        """
        if self.isEmpty():
            self.root = Node(newItem[0], newItem[1])
            return True
        # If the item is already in the tree, you can't insert it again
        if self.retrieveItem(newItem[0])[1]:  # Fix: Pass the key (newItem[0]) instead of the tuple
            return False
        current = self.root
        while current is not None:
            if current.amountOfChildren() > 0:  # Traverse until we reach a leaf
                if current.kv2 != (None, None):  # If it's a 2-node
                    if newItem[0] < current.kv1[0]:  # Fix: Compare the key (newItem[0])
                        current = current.LeftChild
                    elif current.kv1[0] <= newItem[0] < current.kv2[0]:
                        current = current.MiddleChild
                    else:
                        current = current.RightChild
                else:  # If it's a 1-node
                    if newItem[0] < current.kv1[0]:
                        current = current.LeftChild
                    else:
                        current = current.RightChild
            else:
                break  # Stop if we reach a leaf node
        # At this point, current should be a leaf node
        if current is not None:
            if current.kv2 == (None, None):
                current.addValue(newItem[0], newItem[1])
            else:
                # Leaf node is full, split is required
                self._checkAndSplit(current, newItem)
        return True
    def _checkAndSplit(self, node, newItem):
        """
        Check if a node needs to be split, and handle the split operation.
        """
        # Collect values and sort them
        values = [node.kv1, node.kv2, newItem]
        values.sort()  # Sort to find the middle value
        middleValue = values[1]
        # Create two new child nodes for the lower and higher values
        leftChild = Node(values[0][0], values[0][1])  # Node with smallest value
        rightChild = Node(values[2][0], values[2][1])  # Node with largest value
        # Reassign children from the original node
        leftChild.LeftChild = node.LeftChild
        leftChild.RightChild = node.MiddleChild
        rightChild.LeftChild = node.MiddleChild
        rightChild.RightChild = node.RightChild
        # Update parent pointers for the new children
        if leftChild.LeftChild:
            leftChild.LeftChild.parent = leftChild
        if leftChild.RightChild:
            leftChild.RightChild.parent = leftChild
        if rightChild.LeftChild:
            rightChild.LeftChild.parent = rightChild
        if rightChild.RightChild:
            rightChild.RightChild.parent = rightChild
        # Update the current node to hold the middle value
        node.kv1 = middleValue
        node.kv2 = (None, None)  # Remove the second value as it's moved up
        node.LeftChild = leftChild
        node.RightChild = rightChild
        node.MiddleChild = None
        # Set the parent for the new children
        leftChild.parent = node
        rightChild.parent = node
        # If the node has a parent, handle the parent's split recursively
        if node.parent is not None:
            self._checkAndSplitParent(node, leftChild, rightChild)
        else:
            # If the node is root, create a new root
            newRoot = Node(node.kv1[0], node.kv1[1])
            newRoot.LeftChild = leftChild
            newRoot.RightChild = rightChild
            leftChild.parent = newRoot
            rightChild.parent = newRoot
            self.root = newRoot
    def _checkAndSplitParent(self, node, leftChild, rightChild):
        """
        Handle splitting of the parent node during recursive splits.
        """
        parent = node.parent
        # Insert the middle value into the parent
        if parent.kv2 == (None, None):  # Parent has space for one more value
            if node == parent.LeftChild:  # Current node is the left child
                parent.kv2 = parent.kv1  # Shift parent kv1 to kv2
                parent.kv1 = node.kv1  # Promote the middle value
                parent.MiddleChild = rightChild
                parent.LeftChild = leftChild
            else:  # Current node is the right child
                if node.kv2 != (None, None):
                    parent.kv2 = node.kv2
                else:
                    parent.kv2 = node.kv1
                parent.MiddleChild = leftChild
                parent.RightChild = rightChild
            # Update parent pointers for the new children
            leftChild.parent = parent
            rightChild.parent = parent
        else:  # Parent is full and needs to be split
            values = [parent.kv1, parent.kv2, node.kv1]
            values.sort()  # Find the middle value to promote
            middleValue = values[1]
            # Determine how to split the children
            if node == parent.LeftChild:
                newLeftChild = Node(values[0][0], values[0][1])
                newRightChild = Node(values[2][0], values[2][1])
                newLeftChild.LeftChild = leftChild
                newLeftChild.RightChild = rightChild
                newRightChild.LeftChild = parent.MiddleChild
                newRightChild.RightChild = parent.RightChild
            elif node == parent.MiddleChild:
                newLeftChild = Node(parent.kv1[0], parent.kv1[1])
                newRightChild = Node(parent.kv2[0], parent.kv2[1])
                newLeftChild.LeftChild = parent.LeftChild
                newLeftChild.RightChild = leftChild
                newRightChild.LeftChild = rightChild
                newRightChild.RightChild = parent.RightChild
            else:  # node == parent.RightChild
                newLeftChild = Node(parent.kv1[0], parent.kv1[1])
                newRightChild = Node(values[2][0], values[2][1])
                newLeftChild.LeftChild = parent.LeftChild
                newLeftChild.RightChild = parent.MiddleChild
                newRightChild.LeftChild = leftChild
                newRightChild.RightChild = rightChild
            # Update parent pointers for new children
            for child in [newLeftChild.LeftChild, newLeftChild.RightChild]:
                if child:
                    child.parent = newLeftChild
            for child in [newRightChild.LeftChild, newRightChild.RightChild]:
                if child:
                    child.parent = newRightChild
            # Recursively split the parent's parent if necessary
            if parent.parent is not None:
                self._checkAndSplitParent(parent, newLeftChild, newRightChild)
            else:
                # Create a new root if the parent was the root
                newRoot = Node(middleValue[0], middleValue[1])
                newRoot.LeftChild = newLeftChild
                newRoot.RightChild = newRightChild
                newLeftChild.parent = newRoot
                newRightChild.parent = newRoot
                self.root = newRoot
    def inorderTraverseValues(self, visit):
        """
        Inorder traversal visits all nodes in the tree and executes a function on them.
        :param visit: (function)
        """
        self._inorderTraverseValuesRec(self.root, visit)
    def _inorderTraverseValuesRec(self, node, visit):
        if node is not None:
            # If the node has a LeftChild, recursively visit it first
            if node.LeftChild is not None:
                self._inorderTraverseValuesRec(node.LeftChild, visit)
            # Visit the first value (kv1)
            visit(node.kv1[1])
            # If there is a MiddleChild, visit it after the first value
            if node.MiddleChild:  # Only for 1-node
                self._inorderTraverseValuesRec(node.MiddleChild, visit)
            # If it's a 2-node, visit the second value and then the RightChild
            if node.kv2 != (None, None):
                visit(node.kv2[1])
            if node.RightChild != (None, None):
                self._inorderTraverseValuesRec(node.RightChild, visit)

    def inorderTraverseKeys(self, visit):
        """
        Inorder traversal visits all nodes in the tree and executes a function on them.
        :param visit: (function)
        """
        self._inorderTraverseKeysRec(self.root, visit)
    def _inorderTraverseKeysRec(self, node, visit):
        if node is not None:
            # If the node has a LeftChild, recursively visit it first
            if node.LeftChild is not None:
                self._inorderTraverseKeysRec(node.LeftChild, visit)
            # Visit the first key (kv1)
            visit(node.kv1[0])
            # If there is a MiddleChild, visit it after the first value
            if node.MiddleChild:  # Only for 1-node
                self._inorderTraverseKeysRec(node.MiddleChild, visit)
            # If it's a 3-node, visit the second key and then the RightChild
            if node.kv2 != (None, None):
                visit(node.kv2[0])
            if node.RightChild != (None, None):
                self._inorderTraverseKeysRec(node.RightChild, visit)

    def retrieveItem(self, searchKey):
        """
        Searches for a specific item in a search tree
        :return: (value: KeyType, found: boolean)
        """
        node, found = self._retrieveRec(self.root, searchKey)
        if not found:
            return None, False
        if searchKey == node.kv1[0]:
            return node.kv1[1], True
        if searchKey == node.kv2[0]:
            return node.kv2[1], True
    def _retrieveRec(self, node, searchKey):
        if node is None:
            return None, False
        # Check for kv1 and kv2 in the current node (1-node or 2-node)
        if node.kv1[0] == searchKey:
            return node, True
        elif node.kv2 != (None, None) and node.kv2[0] == searchKey:  # Only check kv2 if it exists
            return node, True
        # If it's a leaf node (1-node or 2-node) and key is not found
        if node.amountOfChildren() == 0:
            return None, False
        # Handle internal nodes
        if node.kv2 != (None, None):  # 2-node (has kv1 and kv2)
            if searchKey < node.kv1[0]:
                return self._retrieveRec(node.LeftChild, searchKey)
            elif searchKey < node.kv2[0]:
                return self._retrieveRec(node.MiddleChild, searchKey)
            else:
                return self._retrieveRec(node.RightChild, searchKey)
        else:  # 1-node (only kv1 exists)
            if searchKey < node.kv1[0]:
                return self._retrieveRec(node.LeftChild, searchKey)
            else:
                return self._retrieveRec(node.RightChild, searchKey)
    def save(self):
        """
        Saves the red-black tree as a dictionary
        :return: (tree:dict)
        """
        def serialize(node):
            if node is None:
                return None
            # Create the current node dictionary
            node_dict = {'root': [value for value in [node.kv1, node.kv2] if value is not None and value != (None, None)]}
            children = []
            if node.LeftChild:
                children.append(serialize(node.LeftChild))
            if node.MiddleChild:
                children.append(serialize(node.MiddleChild))
            if node.RightChild:
                children.append(serialize(node.RightChild))
            if children:
                node_dict['children'] = children
            return node_dict
        return serialize(self.root)
    def load(self, tree_dict):
        """
        Loads a dictionary into a red-black tree
        :param tree_dict: (dict:dict)
        :return: (success:boolean)
        """
        def deserialize(data):
            if data is None:
                return None
            # Create the current node
            values = data['root']
            node = Node(*values, *values)  # Unpack the root values
            if 'children' in data:
                children = data['children']
                if len(children) > 0:
                    node.LeftChild = deserialize(children[0])
                    if node.LeftChild:
                        node.LeftChild.parent = node
                if len(children) > 1:
                    node.RightChild = deserialize(children[1])
                    if node.RightChild:
                        node.RightChild.parent = node
                # Leave the middle child empty when there are 2 children
                if len(children) == 2:
                    node.MiddleChild = None
                elif len(children) > 2:
                    node.MiddleChild = deserialize(children[2])
                    if node.MiddleChild:
                        node.MiddleChild.parent = node
            return node
        self.root = deserialize(tree_dict)
        return True
    def deleteItem(self, key):
        """
        Removes the element with key "key" from the 2-3 tree.
        precondition: the search tree contains n > 0 elements
        postcondition: the search tree contains n - 1 elements
        :param key: searchkey to delete
        :return: success (boolean)
        """
        # Search the node in which the element is stored, if it doesn't exist, return false
        node, found = self._retrieveRec(self.root, key)
        if found is False:
            return False
        # If the node isn't a leaf, find the inorderSuccessor of the node
        inorderSuccessor = self._findInorderSuccessor(node, key)
        if node.kv1[0] == key:
            nodetuple = node.kv1
        elif node.kv2[0] == key:
            nodetuple = node.kv2
        else:
            print("Fout in TTT Mathijs, nodetuple not found")
            return False
        # If the node is a leaf, it's its own inorderSuccessor
        if inorderSuccessor is not None:
            node.swapValue(nodetuple, inorderSuccessor.kv1)
        else:
            inorderSuccessor = node
        inorderSuccessor.removeValue(inorderSuccessor.kv1[0])
        inorderSuccessor.fixValues()
        inorderSuccessor.fixChildren()
        if inorderSuccessor.parent:
            inorderSuccessor.parent.fixChildren()
        # Fix the tree if the node underflows
        if inorderSuccessor.amountOfValues() == 0:
            self.fix(inorderSuccessor)
        # If the root is now empty, delete it
        if self.root.amountOfValues() == 0:
            if self.root.amountOfChildren() == 1:
                if self.root.LeftChild:
                    self.root.LeftChild.parent = None
                    self.root = self.root.LeftChild
                elif self.root.MiddleChild:
                    self.root.MiddleChild.parent = None
                    self.root = self.root.MiddleChild
                elif self.root.RightChild:
                    self.root.RightChild.parent = None
                    self.root = self.root.RightChild
                self.root.parent = None
            elif self.root.amountOfChildren() == 0:
                self.root = None
        return True

    def _findInorderSuccessor(self, node, key):
        if node.kv1[0] == key:
            if node.MiddleChild is not None:
                return self._findMin(node.MiddleChild)
            if node.RightChild is not None:
                return self._findMin(node.RightChild)
        elif node.kv2[0] == key:
                return self._findMin(node.RightChild)
        if node.amountOfChildren() == 0:
            return None
        current = node
        while current.parent is not None:
            if current.parent.LeftChild == current:
                return current.parent
            current = current.parent
        return None

    def _findMin(self, node):
        """
        Helper method to find the leftmost (smallest) node in a subtree.
        """
        current = node
        while current.LeftChild is not None:
            current = current.LeftChild
        return current

    def fix(self, node):
        current = node
        while current is not None:
            if current.amountOfValues() > 0:
                return  # No underflow, stop fixing
            # Try redistribution first
            if current.redistribute():
                if current.parent:
                    current.parent.fixChildren()
                return  # Stop if redistribution fully fixed the issue
            # If redistribution fails, merge
            if current.merge():
                current = current.parent  # Move up if merging happened
            else:
                return  # Stop if merging fails

    def getLength(self):
        """
        Returns the number of key-value pairs in the 2-3 tree.
        :return: length (int)
        """
        return self._getLengthRec(self.root)

    def _getLengthRec(self, node):
        """
        Recursively counts the number of key-value pairs in the subtree rooted at `node`.
        """
        if node is None:
            return 0

        # Count the key-value pairs in the current node
        length = 1  # kv1 is always present
        if node.kv2 != (None, None):  # If kv2 is present, increment the count
            length += 1

        # Recursively count the key-value pairs in the children
        if node.LeftChild is not None:
            length += self._getLengthRec(node.LeftChild)
        if node.MiddleChild is not None:
            length += self._getLengthRec(node.MiddleChild)
        if node.RightChild is not None:
            length += self._getLengthRec(node.RightChild)

        return length

class TwoThreeTreeTable(TwoThreeTree):
    """
       wrapper for two three tree implementation to Table
    """
    def tableIsEmpty(self):
        return self.isEmpty()
    def tableLength(self):
        return self.getLength()
    def tableInsert(self, newItem):
        createTreeItem(newItem[0], newItem[1])
        return self.insertItem(newItem)
    def tableDelete(self, key):
        return self.deleteItem(key)
    def tableRetrieve(self, key):
        return self.retrieveItem(key)
    def traverseTableValues(self, visit):
        self.inorderTraverseValues(visit)
    def traverseTableKeys(self, visit):
        self.inorderTraverseKeys(visit)

"""
import ast
from graphviz import Digraph

def create_tree_graph(node, graph=None, parent=None):
    if graph is None:
        graph = Digraph(comment='Tree', engine='dot')

    # Create a node for the current root value
    node_id = str(id(node))
    graph.node(node_id, label=" ".join(map(str, node['root'])))

    if parent:
        # If parent exists, create an edge from the parent to this node
        graph.edge(parent, node_id)

    # Recursively add children
    if 'children' in node:
        for child in node['children']:
            create_tree_graph(child, graph, node_id)

    return graph


def visualize_tree_from_input():
    # Get the input string from the user
    input_str = input("Enter the tree structure as a Python dictionary: ")

    try:
        # Parse the string to a Python dictionary
        tree_structure = ast.literal_eval(input_str)

        # Validate the structure
        if not isinstance(tree_structure, dict) or 'root' not in tree_structure:
            print("Invalid tree structure. It should be a dictionary with a 'root' key.")
            return

        # Create the Graphviz tree
        graph = create_tree_graph(tree_structure)

        # Render and display the tree to a file
        graph.render('tree_visualization', format='png', view=True)
        print("Tree visualization generated and opened as 'tree_visualization.png'.")

    except (SyntaxError, ValueError) as e:
        print(f"Error parsing input: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


def visualize_tree(tree_structure, output_file='tree_visualization'):

    def create_tree_graph(node, graph=None, parent=None):
        if graph is None:
            graph = Digraph(comment='Tree', engine='dot')
        node_id = str(id(node))
        graph.node(node_id, label=" ".join(map(str, node['root'])))
        if parent:
            graph.edge(parent, node_id)
        if 'children' in node:
            for child in node['children']:
                create_tree_graph(child, graph, node_id)
        return graph

    if not isinstance(tree_structure, dict) or 'root' not in tree_structure:
        raise ValueError("Invalid tree structure. Must be a dictionary with a 'root' key.")

    graph = create_tree_graph(tree_structure)
    file_path = graph.render(output_file, format='png', cleanup=True)
    print(f"Tree visualization saved to {file_path}.")
    return file_path


def print_tree(obj):
    if isinstance(obj, dict) and 'root' in obj:
        try:
            visualize_tree(obj)
        except Exception as e:
            print(f"Error visualizing tree: {e}")
    else:
        print(obj)



table = TwoThreeTreeTable()
print(table.save())
print( table.tableIsEmpty() )
print( table.tableInsert((5, 5)) ) # Integer
print(table.save())
print( table.tableInsert((3, "Hoi")) ) # String
print(table.save())
print( table.tableInsert((8, 3.14))  )# Float
print(table.save())
print( table.tableInsert((10, (1, 2, 3))) ) # Tuple
print(table.save())
print( table.tableInsert((12, ["a", "b", "c"])) ) # List
print(table.save())
print( table.tableInsert((15, {"key": "value"})) ) # Dictionary
print(table.save())
print( table.tableInsert((18, True)) ) # Boolean
print(table.save())
print( table.tableInsert((20, None)) ) # NoneType
print(table.save())

print( table.tableLength() == 8 )# Check total elements
print(table.save())

print( table.tableRetrieve(3) == ("Hoi", True) )
print( table.tableRetrieve(8) == (3.14, True) )
print( table.tableRetrieve(10) == ((1, 2, 3), True) )
print( table.tableRetrieve(12) == (["a", "b", "c"], True) )
print( table.tableRetrieve(15) == ({"key": "value"}, True) )
print( table.tableRetrieve(18) == (True, True) )
print( table.tableRetrieve(20) == (None, True) )

print(table.save())
print( table.tableDelete(5) is True )
print(table.save())
print( table.tableDelete(10) is True )
print(table.save())
print( table.tableDelete(15) is True )
print(table.save())
print( table.tableDelete(18) is True )
print(table.save())

print(table.save())
print( table.tableLength() == 4 ) # Verify count after deletions
print(table.save())

print(table.save())
print( not table.tableInsert((3, "Test")) ) # 3 is already present
print(table.save())
print( table.tableDelete(3) is True )
print(table.save())
print( not table.tableDelete(3) ) # 3 is already deleted
print(table.save())

print(table.save())
print( not table.tableIsEmpty() ) # Should still have elements
print(table.save())

print(table.save())
print( table.tableDelete(8) )
print(table.save())
print( table.tableDelete(12) )
print(table.save())
print( table.tableDelete(20) )
print(table.save())

print(table.save())
print( table.tableIsEmpty() ) # Should be empty after all deletions
#"""