# ADT Stack
## data

class MyStack:
    def __init__(self, max_size):
        """
        Maakt een stack aan met grootte max_size.
        Preconditie: De grootte moet groter zijn dan 0.
        Postconditie: Er is een stack met grootte maxsize en er is een variable top die naar de top van de stack verwijst.
        :return: Er wordt niets gereturn.
        """
        self.items = [None] * max_size
        self.top = -1

    ##functionaliteit
    def isEmpty(self):
        """
        Bepaalt of een stack leeg is.
        Preconditie: Er moet een stack bestaan.
        Postconditie: Er wordt niets veranderdt aan de stack.
        :return: True als de stack leeg is, als de stack niet leeg is returnt het False.
        """
        teller = 0
        for i in self.items:
            if i is None:
                continue
            else:
                teller += 1
        if teller == 0:
            return True
        else:
            return False


    def getTop(self):
        """
        Geeft het element terug dat zich op dat moment op de top van de stack bevindt (dus het laatst toegevoegde element).
        Preconditie: Er moet een niet lege lijst bestaan.
        Postconditie: Het element op de top van de stack wordt gereturnt, en de stack zelf wordt niet aangepast.
        :return: Het element op de top van de stack, en True als het gelukt is en False als dit niet gelukt is.
        """
        if self.isEmpty():
            return False,False
        else:
            return self.items[self.top],True

    def pop(self):
        """
        Verwijdert het laatst toegevoegde element van de stack.
        preconditie: Er moet een niet lege lijst bestaan.
        Postconditie: De stack is met 1 element kleinder en de top is nu het voorlaatste toegevoegde element.
        :return: Het verwijderde element en True als de methode succesvol is afgerond, anders False.
        """
        if self.isEmpty():
            return False,False
        else:
            verwijderd_item = self.items[self.top]
            self.items[self.top] = None
            self.top -= 1
            return verwijderd_item,True

    def push(self, waarde):
        '''
        Voegt een item toe aan de top van een stack.
        Preconditie: Er is nog ruimte op de stack.
        Postconditie: De lengte van de stack is gestegen met 1 en het element aan de top is het laatste toegevoegde item.
        :return: Geeft True terug als de functie succesvol is afgerond anders returnt het False.
        '''
        self.top += 1
        if self.top >= len(self.items):
            self.top -= 1
            return False
        else:
            self.items[self.top] = waarde
            return True

    def save(self):
        """
        Stelt de stack voor als een lijst.
        Preconditie: Er moet een niet lege lijst bestaan.
        Postconditie: De stack wordt niet aangepast.
        :return: Geeft de lijst van de stack terug.
        """
        return self.items[:self.top+1]

    def load(self,lijst):
        """
        Verwijderdt de al bestaande stack en maakt een nieuwe stack met de gekregen elementen, en voegt ze meteen toe. De max_lengte van de stack is het aantal elementen dat het meekrijgt.
        Preconditie: Er bestaat een stack dat verwijderdt kan worden, en er wordt minstens 1 nieuw element meegegeven.
        Postconditie: De oude stack is verwijderdt en er is een nieuwe stack gemaakt met de gegeven elementen.
        :return: Er wordt niets gereturnt.
        """
        for i in range(self.top+1):
            self.pop()
        self.__init__(len(lijst))
        for j in range(len(self.items)):
            self.push(lijst[j])