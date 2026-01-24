class MyQueue:
    def __init__(self, max_size):
        """
        Initialize a new queue
        :in: max_size, the maximum size of the queue
        precondition: queue doesn't exist
        postcondition: queue exists, and is empty
        """
        self.length = 0
        self.max_size = max_size
        self.myQueue = [None] * max_size

    def isEmpty(self):
        """
        Check if queue is empty
        precondition: queue exists
        postcondition: queue has not changed
        :return: True if queue is empty, False otherwise
        """
        return self.length == 0

    def enqueue(self, newItem):
        """
        Add a new item to the back of the queue
        precondition: queue exists and has the space to hold at least one item
        postcondition: queue is one element longer than in the precondition
        :return: (success:boolean)
        """
        if self.length >= self.max_size:
            return False
        n = self.max_size
        # Start from the end and shift elements to the right
        for i in range(n - 1, 0, -1):
            self.myQueue[i] = self.myQueue[i - 1]
        self.myQueue[0] = newItem
        self.length += 1
        return True

    def dequeue(self):
        """
        Delete the last item from the queue (which is the item that was added first), and return it
        precondition: the queue exists and is not empty
        postcondition: the queue is one element shorter than the precondition
        :return: (queueFront:QueueItemType, success:boolean)
        """
        if self.isEmpty():
            return None, False
        self.length -= 1
        queueFront = self.myQueue[self.length]
        length = self.length
        self.myQueue[self.length] = None
        return queueFront, True

    def getFront(self):
        """
        Get the front item from the queue
        precondition: the queue exists and is not empty
        postcondition: queue has not changed
        :return: (queueFront:QueueItemType, success:boolean)
        """
        if self.isEmpty():
            return None, False
        return self.myQueue[self.length - 1], True

    def save(self):
        """
        Return the queue as a list
        """
        out = []
        for i in range(self.length):
            if self.myQueue[i] != None:
                out.append(self.myQueue[i])
        return out

    def load(self, list):
        """
        Load the queue from a list
        """
        # Reset the queue
        self.myQueue = [None] * self.max_size
        self.length = 0
        # Enqueue the items in the list
        for i in range(len(list)):
            self.enqueue(list[len(list) - i -1])

