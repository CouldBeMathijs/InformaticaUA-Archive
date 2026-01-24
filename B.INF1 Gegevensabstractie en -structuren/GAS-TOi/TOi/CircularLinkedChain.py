class Node:
    def __init__(self, value=None):
        self.value = value
        self.next = None
        self.previous = None

class LinkedChain:
    def __init__(self):
        """
        Maak een nieuwe ketting aan, met een headpointer die nergens naar wijst
        precondition: de lijst bestaat niet
        postcondition: de lijst bestaat en is leeg
        """
        self.head = None

    def isEmpty(self):
        """
        Check of de ketting leeg is
        precondition: de lijst bestaat
        postcondition: de lijst is onveranderd
        :return: (isEmpty:boolean)
        """
        return self.head is None

    def getLength(self):
        """
        Geef weer hoe lang een ketting is
        precondition: de lijst bestaat
        postcondition: de lijst is onveranderd
        :return: (length:integer)
        """
        # Als de lijst leeg is, is de lengte 0
        if self.isEmpty():
            return 0
        # Als de lijst leeg is, is de lengte 0
        if self.isEmpty():
            return 0
        # Begin te tellen vanaf de head pointer
        count = 0
        current = self.head
        # En blijf optellen tot (Kijk hier! Een plaats waar do-while handig was geweest, jammer genoeg kan python dat niet)
        while True:
            count += 1
            current = current.next
            # We de volledige cirkel rond zijn geweest
            if current == self.head:
                break
        return count

    def insert(self, position, value):
        """
        Voeg een nieuw item "value", toe op plaats "position" in de lijst
        precondition: de lijst bestaat, position is een geldige positie in de lijst, value is of listType
        postcondition: de lijst is 1 element langer, alle elementen na de positie zijn doorgeschoven
        :param value:listType
        :param position:integer
        :return: (success:boolean)
        """
        # Check of de positie onmogelijk is, zo ja, stop
        if position < 1 or position > self.getLength() + 1:
            return False
        # Maak een nieuwe node aan
        new_node = Node(value)
        # Als de lijst leeg was
        if self.isEmpty():
            #voeg dit element dan toe als eerste element
            self.head = new_node
            new_node.next = new_node
            new_node.previous = new_node
        else:
            # Anders begin je bij het begin
            current = self.head
            # Als de positie 1 is, moet het element toegevoegd worden aan het begin van de lijst
            if position == 1:
                new_node.next = self.head
                new_node.previous = self.head.previous
                self.head.previous.next = new_node
                self.head.previous = new_node
                self.head = new_node
            else:
                # Anders schuif alle elementen tot de positie door
                for _ in range(1, position - 1):
                    current = current.next
                # En voeg hiertussen het nieuwe element toe
                new_node.next = current.next
                new_node.previous = current
                current.next.previous = new_node
                current.next = new_node
        return True

    def delete(self, position):
        """
        Verwijder de waarde op de meegegeven positie uit de lijst
        precondition: de lijst bevat op z'n minst 1 element, positie is een geldige positie in de lijst
        postcondition: de lijst bevat 1 element minder, en alle elementen na de positie zijn naar achter geschoven
        :param position:integer
        :return: (success:boolean)
        """
        # Check of de positie onmogelijk is, zo ja, stop
        if self.isEmpty() or position < 1 or position > self.getLength():
            return False
        # Begin te tellen van
        current = self.head
        # Als we de headnode gaan verwijderen
        if position == 1:
            # Als de opvolger van de headnode, de headnode is, is er maar 1 element
            if current.next == self.head:
                # Maak de lijst helemaal leeg
                self.head = None
            # Anders, laat het laatste element naar het 2 de wijzen, het 2de terug naar de laatste
            else:

                current.previous.next = current.next
                current.next.previous = current.previous
                self.head = current.next  # En schuif de head pointer door naar het nieuwe eerste element
        else:
            # Schuif door tot de juiste positie
            for _ in range(1, position):
                current = current.next
            # Update de voorganger en opvolger zodat er niet meer naar de te verwijderen node verwezen wordt
            current.previous.next = current.next
            current.next.previous = current.previous
        return True


    def retrieve(self, position):
        """
        Zoek welk element op de position-de positie staat
        precondition: positie is een geldige positie in de lijst
        postcondition: de lijst is onveranderd
        :param position:integer
        :return: (success:boolean, value:listType)
        """
        # Als de lijst leeg is of de positie onmogelijk is, stop
        if self.isEmpty() or position < 1 or position > self.getLength():
            return None, False
        # Begin bij het begin
        current = self.head
        # Schuif door tot de gewilde positie
        for _ in range(1, position):
            current = current.next
        # En geef die positie terug
        return current.value, True

    def load(self, items):
        """
        Laad een python lijst in als linked chain
        precondition: de lijst bestaat
        postcondition: de lijst bevat alle, en enkel de elementen uit items
        :param items:pythonList
        """
        # Wis de lijst, voor als er al elementen in zouden zitten
        self.head = None
        # Voeg elk item in de lijst 1 voor 1 toe aan de ketting
        for item in items:
            self.insert(self.getLength()+1, item)

    def save(self):
        """
        Sla de linked chain op als python list
        precondition: de lijst bestaat
        postcondition: de lijst is onveranderd
        return (output:pythonList)
        """
        # Als de ketting leeg is, geef een lege lijst terug
        if self.isEmpty():
            return []
        # Begin bij het begin van de ketting
        output = []
        current = self.head
        # Blijf het element toevoegen aan de output
        while True:
            output.append(current.value)
            current = current.next
            # Tot we terug bij het begin zijn
            if current == self.head:
                break
        return output

# Testcode
if __name__ == "__main__":
    l = LinkedChain()
    print(l.isEmpty())                      # True
    print(l.getLength())                    # 0
    print(l.retrieve(4)[1])                 # False
    print(l.insert(4,500))    # False
    print(l.isEmpty())                      # True
    print(l.insert(1,500))    # True
    print(l.retrieve(1)[0])                 # 500
    print(l.retrieve(1)[1])                 # True
    print(l.save())                         # [500]
    print(l.insert(1,600))     # True
    print(l.save())                         # [600, 500]
    l.load([10,-9,15])
    l.insert(3,20)
    print(l.delete(0))                      # False
    print(l.save())                         # [10, -9, 20, 15]
    print(l.delete(1))                      # True
    print(l.save())                         # [-9, 20, 15]