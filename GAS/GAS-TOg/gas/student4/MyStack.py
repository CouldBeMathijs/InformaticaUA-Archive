class MyStack:
    def __init__(self, max_size):
        """
        Initialiseer een lege stack.
        - items: Een lijst die de elementen in de stack bijhoudt.
        - top: Houdt bij welk item momenteel bovenaan de stack ligt. Start op -1 voor een lege stack.
        - max_size: De maximale capaciteit van de stack.
        """
        self.items = [None] * max_size
        self.max_size = max_size
        self.top = -1

    def isEmpty(self):
        """Retourneert True als de stack leeg is, anders False."""
        return self.top == -1

    def is_full(self):
        """Retourneert True als de stack vol is, anders False."""
        return self.top == self.max_size - 1

    def push(self, item):
        """
        Voegt een item toe aan de top van de stack.
        Retourneert True als succesvol, of False als de stack vol is.
        """
        if self.is_full():
            return False
        self.top += 1
        self.items[self.top] = item
        return True

    def pop(self):
        """
        Verwijdert en retourneert het item aan de top van de stack.
        Retourneert een tuple (item, True) als succesvol, of (False, False) als de stack leeg is.
        """
        if self.isEmpty():
            return (False, False)
        item = self.items[self.top]
        self.items[self.top] = None
        self.top -= 1
        return (item, True)

    def getTop(self):
        """
        Geeft het item aan de top van de stack zonder het te verwijderen.
        Retourneert een tuple (item, True) als succesvol, of (False, False) als de stack leeg is.
        """
        if self.isEmpty():
            return (False, False)
        return (self.items[self.top], True)

    def save(self):
        """Retourneert de stack als een lijst van items (alleen niet-lege waarden)."""
        return self.items[:self.top + 1]

    def load(self, item_list):
        """
        Laadt items in de stack vanuit een gegeven lijst.
        Past max_size aan indien nodig.
        Retourneert een tuple (True, None) als succesvol.
        """
        if len(item_list) > self.max_size:
            # Vergroot de items lijst en pas max_size aan
            self.items = item_list + [None] * len(item_list)
            self.max_size = len(item_list)
        else:
            self.items = item_list + [None] * (self.max_size - len(item_list))
        self.top = len(item_list) - 1
        return (True, None)