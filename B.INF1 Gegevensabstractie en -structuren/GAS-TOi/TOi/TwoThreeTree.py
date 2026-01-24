class Node:
    """
    A Node in the 2-3 tree
    """
    def __init__(self, value1=None, value2=None):
        self.value1 = value1
        self.value2 = value2
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
        if self.value1:
            valnum += 1
        if self.value2:
            valnum += 1
        return valnum

    def addValue(self, value):
        """
        Adds a value to the node, ensuring the values remain sorted.
        """
        if self.value1 is None:
            self.value1 = value
        elif self.value2 is None:
            # Place the value in the correct slot
            if value < self.value1:
                self.value2 = self.value1
                self.value1 = value
            elif value > self.value1:
                self.value2 = value
        else:
            raise ValueError("Cannot add value to a node with both slots filled")

        # Ensure values are sorted after the addition
        if self.value2:
            if self.value1 > self.value2:
                self.value1, self.value2 = self.value2, self.value1

    def removeValue(self, value):
        """
        Removes a value from a node, ensuring the values remain sorted.
        """
        if self.value1 == value:
            self.value1 = self.value2
            self.value2 = None
            return True
        if self.value2 == value:
            self.value2 = None
            return True
        return False

    def swapValue(self, toDelete, toAdd):
        """
        Swaps 2 values in a node, ensuring the values remain sorted.
        """
        success = False
        if self.value1 == toDelete:
            self.value1 = toAdd
            success = True
        if self.value2 == toDelete:
            self.value2 = toAdd
            success = True
        if self.value2 is None:
            return success
        if self.value1 > self.value2:
            temp = self.value1
            self.value1 = self.value2
            self.value2 = temp
        return success

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
                if child.value1:
                    if child.value1 < self.value1:
                        self.LeftChild = child
                    if child.value1 > self.value1:
                        self.RightChild = child
            return
        # If the redistribution caused 3 children to be put on a 2-Node, fix this
        if len(children) == 3 and self.amountOfValues() == 1:
            if self.LeftChild.amountOfValues() == 1 and self.MiddleChild.amountOfValues() == 1 and self.LeftChild.value1 < self.value1 and self.MiddleChild.value1 < self.value1:
                self.LeftChild.mergeNodes(self.MiddleChild)
            if self.MiddleChild.amountOfValues() == 1 and self.RightChild.amountOfValues() == 1 and self.MiddleChild.value1 > self.value1 and self.RightChild.value1 > self.value1:
                self.MiddleChild.mergeNodes(self.RightChild)
        if len(children) == 3 and self.amountOfValues() == 2:
            for i, child in enumerate(children):
                if child.value1:
                    # If the child has a value smaller than self.value1, it should be on the left
                    if child.value1 < self.value1:
                        if self.LeftChild and self.LeftChild.value1 < self.value1 and not self.LeftChild.value2:
                            # Call mergeNodes if the left child can absorb the current child
                            self.LeftChild.mergeNodes(child)
                        else:
                            # Otherwise, assign it directly to the LeftChild
                            self.LeftChild = child

                    # If the child value is between self.value1 and self.value2, it should be in the middle
                    elif self.value1 < child.value1 < self.value2:
                        self.MiddleChild = child

                    # If the child's value is greater than self.value2, it should go to the right
                    elif child.value1 > self.value2:
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
        self.addValue(sibling.value1)
        if sibling.value2 and not self.value2:  # If the sibling has a second value, add it too.
            self.addValue(sibling.value2)

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

    def fixValues(self):
        """
        Make sure the values in a Node are sorted
        """
        if self.value2 and not self.value1:
            self.value1 = self.value2
            self.value2 = None
        if self.value2 and self.value1 > self.value2:
            temp = self.value1
            self.value1 = self.value2
            self.value2 = temp

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
                        # Take parent's value1, give sibling's highest to parent
                        self.addValue(parent.value1)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value1, sibling.removeHighest())
                    elif self.isRightChild():
                        # Take parent's value2, give sibling's highest to parent
                        self.addValue(parent.value2)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value2, sibling.removeHighest())
                elif sibling.isMiddleChild():
                    if self.isLeftChild():
                        # Take parent's value1, give sibling's lowest to parent
                        self.addValue(parent.value1)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value1, sibling.removeLowest())
                    elif self.isRightChild():
                        # Take parent's value2, give sibling's highest to parent
                        self.addValue(parent.value2)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value2, sibling.removeHighest())
                elif sibling.isRightChild():
                    if self.isMiddleChild():
                        # Take parent's value2, give sibling's lowest to parent
                        self.addValue(parent.value2)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value2, sibling.removeLowest())
                    elif self.isLeftChild():
                        # Take parent's value1, give sibling's lowest to parent
                        self.addValue(parent.value1)
                        if parent.parent is None:
                            parent.removeValue(parent.value1)
                        else:
                            parent.swapValue(parent.value1, sibling.removeLowest())
                return True
        parent.fixChildren()
        # If no sibling can redistribute, return False
        return False

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
                    parent_value = parent.value1
                elif self.isMiddleChild():
                    parent_value = parent.value1 if sibling.isLeftChild() else parent.value2
                elif self.isRightChild():
                    if parent.value2:
                        parent_value = parent.value2
                    else:
                        parent_value = parent.value1
                else:
                    return False

                # Merge parent's value and sibling's values into self
                self.addValue(parent_value)
                if sibling.value1:
                    self.addValue(sibling.value1)
                if sibling.value2:
                    self.addValue(sibling.value2)

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
                parent.removeValue(parent_value)
                parent.fixChildren()
                self.fixChildren()

                # If parent underflows, handle recursively
                if parent and parent.amountOfValues() == 0:
                    parent.merge()
                return True

        # If no sibling to merge with, return False
        return False

    def siblings(self):
        """
        Returns a list of every sibling of the current Node
        """
        if self.parent is None:
            return [] #The root has no siblings
        return [child for child in [self.parent.LeftChild, self.parent.MiddleChild, self.parent.RightChild] if child and child != self]

    def children(self):
        """
        Returns an array with all the children of a Node
        """
        return [child for child in [self.LeftChild, self.MiddleChild, self.RightChild] if child]

    def removeHighest(self):
        """
        Delete the highest value in a Node and return it
        """
        returnval = None
        if self.value2:
            returnval = self.value2
            self.removeValue(self.value2)
        else:
            returnval = self.value1
            self.removeValue(self.value1)
        return returnval

    def removeLowest(self):
        """
        Delete the lowest value in a Node, and return it
        """
        returnval = self.value1
        self.removeValue(self.value1)
        return returnval

def createTreeItem(key, val):
    return val

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
            self.root = Node(newItem)
            return True
        # If the item is already in the tree, you can't insert it again
        if self.retrieveItem(newItem)[1]:
            return False
        current = self.root
        while current is not None:
            if current.amountOfChildren() > 0:  # Traverse until we reach a leaf
                if current.value2 is not None:  # If it's a 2-node
                    if newItem < current.value1:
                        current = current.LeftChild
                    elif current.value1 <= newItem < current.value2:
                        current = current.MiddleChild
                    else:
                        current = current.RightChild
                else:  # If it's a 1-node
                    if newItem < current.value1:
                        current = current.LeftChild
                    else:
                        current = current.RightChild
            else:
                break  # Stop if we reach a leaf node

        # At this point, current should be a leaf node
        if current is not None:
            if current.value2 is None:
                current.addValue(newItem)
            else:
                # Leaf node is full, split is required
                self._checkAndSplit(current, newItem)
        return True

    def _checkAndSplit(self, node, newItem):
        """
        Check if a node needs to be split, and handle the split operation.
        """
        # Collect values and sort them
        values = [node.value1, node.value2, newItem]
        values.sort()  # Sort to find the middle value
        middleValue = values[1]

        # Create two new child nodes for the lower and higher values
        leftChild = Node(values[0])  # Node with smallest value
        rightChild = Node(values[2])  # Node with largest value

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
        node.value1 = middleValue
        node.value2 = None  # Remove the second value as it's moved up
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
            newRoot = Node(node.value1)
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
        if parent.value2 is None:  # Parent has space for one more value
            if node == parent.LeftChild:  # Current node is the left child
                parent.value2 = parent.value1  # Shift parent value1 to value2
                parent.value1 = node.value1  # Promote the middle value
                parent.MiddleChild = rightChild
                parent.LeftChild = leftChild
            else:  # Current node is the right child
                if node.value2:
                    parent.value2 = node.value2
                else:
                    parent.value2 = node.value1
                parent.MiddleChild = leftChild
                parent.RightChild = rightChild
            # Update parent pointers for the new children
            leftChild.parent = parent
            rightChild.parent = parent
        else:  # Parent is full and needs to be split
            values = [parent.value1, parent.value2, node.value1]
            values.sort()  # Find the middle value to promote
            middleValue = values[1]

            # Determine how to split the children
            if node == parent.LeftChild:
                newLeftChild = Node(values[0])
                newRightChild = Node(values[2])
                newLeftChild.LeftChild = leftChild
                newLeftChild.RightChild = rightChild
                newRightChild.LeftChild = parent.MiddleChild
                newRightChild.RightChild = parent.RightChild
            elif node == parent.MiddleChild:
                newLeftChild = Node(parent.value1)
                newRightChild = Node(parent.value2)
                newLeftChild.LeftChild = parent.LeftChild
                newLeftChild.RightChild = leftChild
                newRightChild.LeftChild = rightChild
                newRightChild.RightChild = parent.RightChild
            else:  # node == parent.RightChild
                newLeftChild = Node(parent.value1)
                newRightChild = Node(values[2])
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
                newRoot = Node(middleValue)
                newRoot.LeftChild = newLeftChild
                newRoot.RightChild = newRightChild

                newLeftChild.parent = newRoot
                newRightChild.parent = newRoot
                self.root = newRoot

    def inorderTraverse(self, visit):
        """
        Inorder traversal visits all nodes in the tree and executes a function on them.
        :param visit: (function)
        """
        self._inorderTraverseRec(self.root, visit)

    def _inorderTraverseRec(self, node, visit):
        if node is not None:
            # If the node has a LeftChild, recursively visit it first
            if node.LeftChild is not None:
                self._inorderTraverseRec(node.LeftChild, visit)

            # Visit the first value (value1)
            visit(node.value1)

            # If there is a MiddleChild, visit it after the first value
            if node.MiddleChild:  # Only for 1-node
                self._inorderTraverseRec(node.MiddleChild, visit)



            # If it's a 2-node, visit the second value and then the RightChild
            if node.value2 is not None:
                visit(node.value2)
            if node.RightChild is not None:
                self._inorderTraverseRec(node.RightChild, visit)


    def retrieveItem(self, searchKey):
        """
        Searches for a specific item in a search tree
        :return: (value: KeyType, found: boolean)
        """
        node, found = self._retrieveRec(self.root, searchKey)
        if not found:
            return None, False
        if searchKey == node.value1:
            return node.value1, True
        if searchKey == node.value2:
            return node.value2, True

    def _retrieveRec(self, node, searchKey):
        if node is None:
            return None, False

        # Check for value1 and value2 in the current node (1-node or 2-node)
        if node.value1 == searchKey:
            return node, True
        elif node.value2 is not None and node.value2 == searchKey:  # Only check value2 if it exists
            return node, True

        # If it's a leaf node (1-node or 2-node) and key is not found
        if node.amountOfChildren() == 0:
            return None, False

        # Handle internal nodes
        if node.value2 is not None:  # 2-node (has value1 and value2)
            if searchKey < node.value1:
                return self._retrieveRec(node.LeftChild, searchKey)
            elif searchKey < node.value2:
                return self._retrieveRec(node.MiddleChild, searchKey)
            else:
                return self._retrieveRec(node.RightChild, searchKey)
        else:  # 1-node (only value1 exists)
            if searchKey < node.value1:
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
            node_dict = {'root': [value for value in [node.value1, node.value2] if value is not None]}
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
            node = Node(*values)  # Unpack the root values

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
        # If the node is a leaf, it's its own inorderSuccessor
        if inorderSuccessor is not None:
            node.swapValue(key, inorderSuccessor.value1)
        else:
            inorderSuccessor = node
        inorderSuccessor.removeValue(inorderSuccessor.value1)
        inorderSuccessor.fixValues()
        inorderSuccessor.fixChildren()
        if inorderSuccessor.parent:
            inorderSuccessor.parent.fixChildren()

        #Fix the tree if the node underflows
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
        if node.amountOfChildren() == 0:
            return None
        # Case 1: Node has a right child
        if node.RightChild is not None:
            return self._findMin(node.RightChild)

        # Case 2: Node has no right child
        # Traverse upwards to find the first ancestor where the node is in the left subtree
        current = node
        while current.parent is not None:
            if current.parent.LeftChild == current:
                return current.parent
            current = current.parent

        # If we reach here, there is no successor (node is the largest element)
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
