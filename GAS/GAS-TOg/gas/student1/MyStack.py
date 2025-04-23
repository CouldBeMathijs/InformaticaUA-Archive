class MyStack:
    def __init__(self, max_size):
        """
        Create a new stack
        :in: max_size, the maximum size of the stack
        precondition: stack doesn't exist
        postcondition: stack exists, and is empty
        """
        self.max_size = max_size
        self.length = 0
        self.myStack = [None] * self.max_size
    def isEmpty(self):
        """
        Check if stack is empty
        precondition: stack exists
        postcondition: stack has not changed
        :return: True if stack is empty, False otherwise
        """
        return self.length == 0
    def push(self, newItem):
        """
        Add newItem to the top of the stack
        precondition: stack exists and has the space to hold at least one item
        postcondition: stack is one element longer than in the precondition
        :return: (succes:boolean)
        """
        if self.length >= self.max_size:
            return False
        self.myStack[self.length] = newItem
        self.length += 1
        return True
    def pop(self):
        """
        Pop the top item from the stack (the item that was added last)
        precondition: the stack exists and is not empty
        postcondition: the stack is one element shorter than the precondition
        :return: (stackFront:stackItemType, success:boolean)
        """
        if self.isEmpty():
            return None, False
        self.length -= 1
        stackFront = self.myStack[self.length]
        self.myStack[self.length] = None
        return stackFront, True

    def getTop(self):
        """
        Get the top item from the stack
        precondition: the stack exists and is not empty
        postcondition: stack has not changed
        :return: (stackTop:StackItemType, success:boolean)
        """
        if self.isEmpty():
            return None, False
        return self.myStack[self.length-1], True
    def save(self):
        """
        Return the stack as a list
        """
        out = []
        for i in range(self.length):
            if self.myStack[i] != None:
                out.append(self.myStack[i])
        return out
    def load(self, list):
        """
        Load the stack from a list
        """
        # Reset the stack
        self.max_size = len(list)
        self.myStack = [None] * self.max_size
        self.length = 0
        # Push the items in the list to the stack
        for item in list:
            self.push(item)
