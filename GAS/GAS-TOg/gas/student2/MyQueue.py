# ADT queue
## data
import copy


class MyQueue:
    def __init__(self, max_size):
        """
        Maakt een queue aan met grootte max_size.
        Preconditie: De grootte moet groter zijn dan 0.
        Postconditie: Er is een queue met grootte maxsize en er is een variable kop die naar de kop van de queue verwijst (het eerst toegevoegde element).
        :return: Er wordt niets gereturnt.
        """
        self.items = [None] * max_size
        self.kop = -1

    ##functionaliteit
    def isEmpty(self):
        """
        Bepaalt of een queue leeg is.
        Preconditie: Er moet een queue bestaan.
        Postconditie: Er wordt niets veranderd aan de queue.
        :return: True als de queue leeg is, als de queue niet leeg is returnt het False.
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

    def getFront(self):
        """
        Geeft het element terug dat zich op dat moment aan de kop van de queue bevindt (dus het eerste toegevoegde element).
        Preconditie: Er moet een niet lege queue bestaan.
        Postconditie: het element op de kop van de queue wordt gereturnt, en de queue zelf wordt niet aangepast.
        :return: Het element op de kop van de queue, en True als het gelukt is en False als dit niet gelukt is.
        """
        if self.isEmpty():
            return False, False
        else:
            return self.items[self.kop], True

    def dequeue(self):
        """
        Verwijdert het eerste toegevoegde element van de queue (ook weld e kop genoemd).
        Preconditie: Er moet een niet lege queue bestaan.
        Postconditie: De queue is met 1 element kleinder en de kop van de queue is nu het tweede toegevoegde element.
        :return: Het verwijderde element en True als de methode succesvol is afgerond, anders False.
        """
        if self.isEmpty():
            return False, False
        else:
            verwijderd_item = self.items[self.kop]
            self.items[self.kop] = None
            self.kop -= 1
            return verwijderd_item, True

    def enqueue(self, waarde):
        '''
        Voegt een item toe aan de kop van een queue.
        Preconditie: Er is nog ruimte op de queue.
        Postconditie: De lengte van de queue is gestegen met 1 en het element aan de kop is het tweede toegevoegde item.
        :return: Geeft True terug als de functie succesvol is afgerond anders returnt het False.
        '''
        self.kop += 1
        if len(self.items) - 1 >= len(self.items):
            self.kop -= 1
            return False
        else:
            extra_lijst = copy.copy(self.items)
            for i in range(len(self.items) - 1):
                self.items[i + 1] = extra_lijst[i]
            self.items[0] = waarde
            return True

    def save(self):
        """
        Stelt de queue voor als een lijst.
        Preconditie: Er moet een niet lege queue bestaan.
        Postconditie: De queue wordt niet aangepast.
        :return: Geeft de lijst van de queue terug.
        """
        return self.items[:self.kop + 1]

    def load(self, lijst):
        """
        Verwijdert de al bestaande queue en maakt een nieuwe queue met de gekregen elementen, en voegt ze meteen toe. De max_lengte van de queue is het aantal elementen dat het meekrijgt.
        Preconditie: Er bestaat een queue dat verwijderd kan worden, en er wordt minstens 1 nieuw element meegegeven.
        Postconditie: De oude queue is verwijderd en er is een nieuwe queue gemaakt met de gegeven elementen.
        :return: Er wordt niets gereturnt.
        """
        for i in range(self.kop + 1):
            self.dequeue()
        self.__init__(len(lijst))
        for j in range(len(self.items)):
            self.enqueue(lijst[len(self.items) - j - 1])
