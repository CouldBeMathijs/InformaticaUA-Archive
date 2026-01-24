class MyQueue:
    def __init__(self, max_size):
        """
        Initialiseer een lege queue.
        - items: Een lijst om de elementen in de queue op te slaan.
        - front: Houdt bij waar het eerste element zich bevindt, start op 0.
        - rear: Houdt bij waar het laatste element zich bevindt, start op -1 voor een lege queue.
        - size: Houdt de huidige grootte van de queue bij, start op 0.
        - max_size: De maximale capaciteit van de queue.
        """
        self.items = [None] * max_size
        self.max_size = max_size
        self.front = 0
        self.rear = -1
        self.size = 0

    def isEmpty(self):
        """Retourneert True als de queue leeg is, anders False."""
        return self.size == 0

    def isFull(self):
        """Retourneert True als de queue vol is, anders False."""
        return self.size == self.max_size

    def enqueue(self, item):
        """
        Voegt een item toe aan het einde van de queue.
        Preconditie: de queue mag niet vol zijn.
        Postconditie: de queue bevat één item meer, en het toegevoegde item is achteraan.
        """
        if self.isFull():
            return False
        self.rear = (self.rear + 1) % self.max_size
        self.items[self.rear] = item
        self.size += 1
        return True

    def dequeue(self):
        """
        Verwijdert en retourneert het item aan het begin van de queue.
        Preconditie: de queue mag niet leeg zijn.
        Postconditie: de queue bevat één item minder, en het verwijderde item is niet meer beschikbaar.
        """
        if self.isEmpty():
            return [False, False]
        item = self.items[self.front]
        self.items[self.front] = None
        self.front = (self.front + 1) % self.max_size
        self.size -= 1
        return [item, True]

    def getFront(self):
        """
        Geeft het item aan het begin van de queue zonder het te verwijderen.
        Preconditie: de queue mag niet leeg zijn.
        """
        if self.isEmpty():
            return [False, False]
        return [self.items[self.front], True]

    def save(self):
        """Retourneert de queue als een lijst van huidige items (alleen niet-lege waarden)."""
        if self.isEmpty():
            return []
        return [self.items[(self.front + i) % self.max_size] for i in range(self.size)[::-1]]

    def load(self, item_list):
        """
        Laadt items in de queue vanuit een gegeven lijst.
        Preconditie: de lijst mag niet langer zijn dan max_size.
        Postconditie: de queue bevat de items uit de gegeven lijst en wordt overschreven.
        """
        if len(item_list) > self.max_size:
            return False
        self.items = item_list[::-1] + [None] * (self.max_size - len(item_list))
        self.front = 0
        self.rear = len(item_list) - 1
        self.size = len(item_list)
        return True