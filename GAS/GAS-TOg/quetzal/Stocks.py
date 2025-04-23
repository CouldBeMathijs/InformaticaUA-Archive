"""
Stocks.py: Beheer van voorraad aan chocoladeshots en toppings.

Precondition:
- De module `quetzal.app` en de vereiste objecten moeten beschikbaar zijn.

Postcondition:
- Objecten van verschillende types kunnen toegevoegd, opgevraagd en gecontroleerd worden.

Auteur: Groep 10
Datum: 25-03-2025
"""

import quetzal.app as app

def chocoshotsoort(chocoshot):
    return chocoshot.type

# ----------------------------------------------------------
# Class: Stocks
# ----------------------------------------------------------
class Stocks:
    """
    Deze klasse beheert de voorraad van chocoladeshots en toppings.

    Parameters:
    - storagesize: De maximale opslagcapaciteit per Queue.

    Precondition:
    - storagesize is een positief geheel getal.

    Postcondition:
    - Alle benodigde Table- en Queue-objecten zijn geïnitialiseerd.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, storagesize):
        """
        Initialiseert de voorraadstructuren per type item.

        Parameters:
        - storagesize (int): Aantal elementen dat per queue opgeslagen kan worden.

        Postcondition:
        - Elke Table en Queue is aangemaakt voor de verschillende items.
        """
        self.chocoladeshotsWit = app.Table()
        self.chocoladeshotsWitKeys = app.Queue(storagesize)
        self.chocoladeshotsBruin = app.Table()
        self.chocoladeshotsBruinKeys = app.Queue(storagesize)
        self.chocoladeshotsMelk = app.Table()
        self.chocoladeshotsMelkKeys = app.Queue(storagesize)
        self.chocoladeshotsZwart = app.Table()
        self.chocoladeshotsZwartKeys = app.Queue(storagesize)
        self.honingporties = app.Table()
        self.honingportiesKeys = app.Queue(storagesize)
        self.marshmallows = app.Table()
        self.marshmallowsKeys = app.Queue(storagesize)
        self.chilipeperporties = app.Table()
        self.chilipeperportiesKeys = app.Queue(storagesize)

        print("[Stocks] Stocks geïnitialiseerd met opslaggrootte:", storagesize)

    def addStock(self, klasse, type, aantal, jaar, maand, dag):
        """
        Voegt items toe aan de voorraad op basis van de klasse.

        Parameters:
        - klasse: Type van het item ('shot', 'honing', 'marshmallow', 'chilipeper')
        - type: Alleen van toepassing bij shots
        - aantal: Aantal toe te voegen
        - jaar, maand, dag: Datumcomponenten

        Precondition:
        - Gegevens zijn geldig en consistent met de klasse

        Postcondition:
        - Item(s) worden toegevoegd aan de juiste Table en Queue.
        """
        print(f"[Stocks] addStock aangeroepen voor klasse '{klasse}' met type '{type}' en aantal {aantal}")
        if klasse == "shot":
            self.__handleShot(type, aantal, jaar, maand, dag)
        elif klasse == "honing":
            for _ in range(aantal):
                id = app.time.time_ns()
                honing = app.Honing(id, int(jaar + maand + dag))
                itemToAdd = (id, honing)
                self.honingportiesKeys.enqueue(itemToAdd[0])
                self.honingporties.tableInsert(itemToAdd)
                print(f"[Stocks] Honing toegevoegd met id {id}")
        elif klasse == "chilipeper":
            for _ in range(aantal):
                id = app.time.time_ns()
                chilipeper = app.Chilipeper(id, int(jaar + maand + dag))
                itemToAdd = (id, chilipeper)
                self.chilipeperportiesKeys.enqueue(itemToAdd[0])
                self.chilipeperporties.tableInsert(itemToAdd)
                print(f"[Stocks] Chilipeper toegevoegd met id {id}")
        elif klasse == "marshmallow":
            for _ in range(aantal):
                id = app.time.time_ns()
                marshmallow = app.Marshmallow(id, int(jaar + maand + dag))
                itemToAdd = (id, marshmallow)
                self.marshmallowsKeys.enqueue(itemToAdd[0])
                self.marshmallows.tableInsert(itemToAdd)
                print(f"[Stocks] Marshmallow toegevoegd met id {id}")

    # ----------------------------------------------------------
    # Interne shot handler
    # ----------------------------------------------------------
    def __handleShot(self, type, aantal, jaar, maand, dag):
        """
        Interne methode om chocoladeshots toe te voegen.

        Precondition:
        - type is geldig ('wit', 'bruin', 'melk', 'zwart')

        Postcondition:
        - Chocoladeshots zijn toegevoegd aan de juiste voorraadstructuren.
        """
        for _ in range(aantal):
            id = app.time.time_ns()
            chocoladeshot = app.Chocoladeshot(id, type, int(jaar + maand + dag))
            itemToAdd = (id, chocoladeshot)
            if type == "wit":
                self.chocoladeshotsWitKeys.enqueue(itemToAdd[0])
                self.chocoladeshotsWit.tableInsert(itemToAdd)
            elif type == "bruin":
                self.chocoladeshotsBruinKeys.enqueue(itemToAdd[0])
                self.chocoladeshotsBruin.tableInsert(itemToAdd)
            elif type == "melk":
                self.chocoladeshotsMelkKeys.enqueue(itemToAdd[0])
                self.chocoladeshotsMelk.tableInsert(itemToAdd)
            elif type == "zwart":
                self.chocoladeshotsZwartKeys.enqueue(itemToAdd[0])
                self.chocoladeshotsZwart.tableInsert(itemToAdd)

            print(f"[Stocks] Chocoladeshot '{type}' toegevoegd met id {id}")

    def getChocoladeshot(self, type):
        """
        Haalt een chocoladeshot op van het opgegeven type.

        Parameters:
        - type: Het type chocoladeshot ('wit', 'bruin', 'melk', 'zwart')

        Postcondition:
        - Het oudste element van het gevraagde type wordt uit de voorraad verwijderd en teruggegeven.
        """
        print(f"[Stocks] Ophalen van chocoladeshot type: {type}")
        if type == "wit":
            return self.chocoladeshotsWit.tableDelete(self.chocoladeshotsWitKeys.dequeue()[0])
        elif type == "bruin":
            return self.chocoladeshotsBruin.tableDelete(self.chocoladeshotsBruinKeys.dequeue()[0])
        elif type == "melk":
            return self.chocoladeshotsMelk.tableDelete(self.chocoladeshotsMelkKeys.dequeue()[0])
        elif type == "zwart":
            return self.chocoladeshotsZwart.tableDelete(self.chocoladeshotsZwartKeys.dequeue()[0])

    def getHoning(self):
        """
        Haalt een honingportie op.

        Postcondition:
        - Het oudste honingobject wordt verwijderd en teruggegeven.
        """
        print("[Stocks] Ophalen van honing")
        return self.honingporties.tableDelete(self.honingportiesKeys.dequeue()[0])
        

    def getMarshmallow(self):
        """
        Haalt een marshmallow op.

        Postcondition:
        - Het oudste marshmallow-object wordt verwijderd en teruggegeven.
        """
        print("[Stocks] Ophalen van marshmallow")
        return self.marshmallows.tableDelete(self.marshmallowsKeys.dequeue()[0])

    def getChilipeper(self):
        """
        Haalt een chilipeperportie op.

        Postcondition:
        - Het oudste chilipeper-object wordt verwijderd en teruggegeven.
        """
        print("[Stocks] Ophalen van chilipeper")
        return self.chilipeperporties.tableDelete(self.chilipeperportiesKeys.dequeue()[0])

    def checkChocoladeshot(self, amount, type):
        """
        Controleert of er voldoende chocoladeshots zijn van het gevraagde type.

        Parameters:
        - amount (int): Gewenst minimumaantal
        - type (str): Het type chocoladeshot

        Return:
        - True als er minstens 'amount' aanwezig zijn, anders False.
        """
        print(f"[Stocks] Controleren of er minstens {amount} chocoladeshots van type '{type}' aanwezig zijn")
        if type == "wit":
            return self.chocoladeshotsWit.getLength() >= amount
        elif type == "bruin":
            return self.chocoladeshotsBruin.getLength() >= amount
        elif type == "melk":
            return self.chocoladeshotsMelk.getLength() >= amount
        elif type == "zwart":
            return self.chocoladeshotsZwart.getLength() >= amount

    def checkHoning(self, amount):
        """
        Controleert of er minstens 'amount' honingporties zijn.

        Return:
        - True als voldoende aanwezig, anders False.
        """
        print(f"[Stocks] Controleren of er minstens {amount} honingporties aanwezig zijn")
        return self.honingporties.getLength() >= amount

    def checkMarshmallow(self, amount):
        """
        Controleert of er minstens 'amount' marshmallows zijn.

        Return:
        - True als voldoende aanwezig, anders False.
        """
        print(f"[Stocks] Controleren of er minstens {amount} marshmallows aanwezig zijn")
        return self.marshmallows.getLength() >= amount

    def checkChilipeper(self, amount):
        """
        Controleert of er minstens 'amount' chilipepers zijn.

        Return:
        - True als voldoende aanwezig, anders False.
        """
        print(f"[Stocks] Controleren of er minstens {amount} chilipeperporties aanwezig zijn")
        return self.chilipeperporties.getLength() >= amount

    # ----------------------------------------------------------
    # Voorraadoverzicht
    # ----------------------------------------------------------
    def getstocks(self):
        """
        Geeft een overzicht van de huidige stock.

        Return:
        - Tuple met het aantal elementen per type voorraad.
        """
        stocks_overzicht = (
            self.chocoladeshotsWit.tableLength(),
            self.chocoladeshotsBruin.tableLength(),
            self.chocoladeshotsMelk.tableLength(),
            self.chocoladeshotsZwart.tableLength(),
            self.honingporties.tableLength(),
            self.marshmallows.tableLength(),
            self.chilipeperporties.tableLength()
        )
        print(f"[Stocks] Huidige stock: {stocks_overzicht}")
        return stocks_overzicht
