"""
System.py: Beheert de reservaties, gebruikers, werknemers en bijbehorende bestellingen.

Precondition:
- Geldige configuraties voor MyStack, MyQueue, BSTTable, Bestelling, Gebruiker, Stocks, Werknemer, en input-module moeten aanwezig zijn.
- De constanten en klassen waarnaar wordt verwezen (zoals MyStack, MyQueue, Table, Bestelling, Gebruiker, etc.) moeten correct geïmplementeerd zijn.

Postcondition:
- Het systeem kan gebruikers, werknemers, bestellingen en stocks beheren.
- Logbestanden kunnen worden aangemaakt en opgeslagen.

Auteur: Groep 10
Datum: 21-2-2025
"""

# ----------------------------------------------------------
# Imports
# ----------------------------------------------------------
import quetzal.app as app

# ----------------------------------------------------------
# Class: System
# ----------------------------------------------------------
class System:
    """
    Deze klasse beheert gebruikers, werknemers, bestellingen en stockbeheer binnen het systeem.

    Parameters:
    - Geen expliciete parameters bij initialisatie.

    Precondition:
    - Externe modules (zoals app.Gebruiker, app.Werknemer, app.Bestelling, enz.) moeten correct functioneren.

    Postcondition:
    - Een volledig functioneel systeemobject is aangemaakt met alle datastructuren klaar voor gebruik.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self):
        """
        Deze klasse beheert gebruikers, werknemers, bestellingen en stockbeheer binnen het systeem.

        Attributen:
        - sizeofsq: Grootte van de stack en queue voor werknemers en bestellingen.
        - werknemers: Opslag van alle werknemers.
        - gebruikers: Opslag van alle gebruikers.
        - beschikbaar: Stack van beschikbare werknemers.
        - bestellingen: Queue met openstaande bestellingen.
        - time: Tijdseenheid van het systeem.
        - afgewerkt: Tabel met afgewerkte bestellingen.
        - stocks: Stockbeheerobject.
        - logs: HTML-representatie van het logbestand.
        - nieuwebestellingen: Tijdelijke opslag van bestellingen geplaatst in deze tick.

        Precondition:
        - Externe modules (zoals app.Gebruiker, app.Werknemer, app.Bestelling, enz.) moeten correct functioneren.

        Postcondition:
        - Een volledig functioneel systeemobject is aangemaakt.
        """

        print("[System] Initialisatie van het systeem")
        self.sizeofsq = 50
        self.werknemers = app.Table()
        self.gebruikers = app.Table()
        self.beschikbaar = app.Stack(self.sizeofsq)
        self.bestellingen = app.Queue(self.sizeofsq)
        self.time = 0
        self.afgewerkt = app.Table()
        self.stocks = app.Stocks(self.sizeofsq)
        self.logs = ""
        self.nieuwebestellingen = []
        print("[System] Systeem succesvol geïnitialiseerd")

    # ----------------------------------------------------------
    # Gebruikersbeheer
    # ----------------------------------------------------------
    def addGebruiker(self, voornaam, achternaam, email):
        """
        Voegt een gebruiker toe aan het systeem.

        Parameters:
        - voornaam: Voornaam van de gebruiker.
        - achternaam: Achternaam van de gebruiker.
        - email: Uniek e-mailadres van de gebruiker.

        Postcondition:
        - De gebruiker is toegevoegd aan de gebruikers-tabel.
        """
        gebruiker = app.Gebruiker(email, voornaam, achternaam, email)
        self.gebruikers.tableInsert((email, gebruiker))
        print(f"[System] Gebruiker {gebruiker.getName()} is toegevoegd aan de lijst met gebruikers")

    # ----------------------------------------------------------
    # Werknemersbeheer
    # ----------------------------------------------------------
    def addWerknemer(self, voornaam, achternaam, werklast):
        """
        Voegt een werknemer toe aan het systeem.

        Parameters:
        - voornaam: Voornaam van de werknemer.
        - achternaam: Achternaam van de werknemer.
        - werklast: Aantal credits dat de werknemer per tick kan verwerken.

        Postcondition:
        - De werknemer is toegevoegd aan de werknemers-tabel en beschikbaarheidsstack.
        """
        werknemerid = app.time.time_ns()
        werknemer = app.Werknemer(werknemerid, voornaam, achternaam, werklast)
        self.beschikbaar.push(werknemer)
        self.werknemers.tableInsert((werknemerid, werknemer))
        print(f"[System] Werknemer {werknemer.getName()} is toegevoegd aan de lijst met werknemers en aan de stack")

    # ----------------------------------------------------------
    # Bestellingenbeheer
    # ----------------------------------------------------------
    def bestel(self, gebruikerEmail, ingredienten, jaar, maand, dag, uur, minuut):
        """
        Plaatst een bestelling namens een gebruiker met opgegeven ingrediënten.

        Parameters:
        - gebruikerEmail: Email van de gebruiker die bestelt.
        - ingredienten: Lijst van ingredientnamen.
        - jaar, maand, dag, uur, minuut: Tijdscomponenten van de bestelling.

        Postcondition:
        - Een Bestelling-object wordt aangemaakt, toegevoegd aan de queue en gelogd in nieuwebestellingen.
        """
        print(f"[System] Bestelling geplaatst door {gebruikerEmail} met ingrediënten {ingredienten}")
        ingredients = []
        for ingredient in ingredienten:
            if ingredient == "chilipeper":
                ingredient_item = self.stocks.getChilipeper()
                ingredients.append(ingredient_item)
            elif ingredient == "marshmallow":
                ingredient_item = self.stocks.getMarshmallow()
                ingredients.append(ingredient_item)
            elif ingredient == "honing":
                ingredient_item = self.stocks.getHoning()
                ingredients.append(ingredient_item)
            elif ingredient == "melk":
                ingredient_item = self.stocks.getChocoladeshot("melk")
                ingredients.append(ingredient_item)
            elif ingredient == "wit":
                ingredient_item = self.stocks.getChocoladeshot("wit")
                ingredients.append(ingredient_item)
            elif ingredient == "bruin":
                ingredient_item = self.stocks.getChocoladeshot("bruin")
                ingredients.append(ingredient_item)
            elif ingredient == "zwart":
                ingredient_item = self.stocks.getChocoladeshot("zwart")
                ingredients.append(ingredient_item)
        bestelling = app.Bestelling(None, gebruikerEmail, str(jaar) + str(maand) + str(dag) + str(uur) + str(minuut), app.time.time_ns(), ingredients)
        self.bestellingen.enqueue(bestelling)
        self.nieuwebestellingen.append(bestelling)
        print(f"[System] Bestelling toegevoegd aan de queue met timestamp {bestelling.timestamp}")

    # ----------------------------------------------------------
    # Behandeling van bestellingen
    # ----------------------------------------------------------
    def behandelbestellingen(self):
        """
        Verwerkt één bestelling door een beschikbare werknemer deze te laten behandelen.

        Postcondition:
        - Een werknemer wordt gekoppeld aan een bestelling en begint deze te verwerken.
        - Retourneert False als er geen werknemer of bestelling beschikbaar is, anders True.
        """
        if self.beschikbaar.isEmpty() or self.bestellingen.isEmpty():
            print("[System] Geen beschikbare werknemers of bestellingen om te behandelen")
            return False
        werknemer = self.beschikbaar.pop()[0]
        bestelling = self.bestellingen.dequeue()[0]
        bestelling.afwerker = werknemer
        werknemer.behandel(bestelling)
        print(f"[System] Werknemer {werknemer.getName()} behandelt bestelling van {bestelling.gebruikersid}")
        return True

    def clearNieuweBestelingen(self):
        """
        Leegt de lijst met nieuw geregistreerde bestellingen.

        Postcondition:
        - De lijst self.nieuwebestellingen is geleegd.
        """
        self.nieuwebestellingen.clear()

    def tick(self):
        """
        Simuleert het verstrijken van één tijdseenheid in het systeem.

        Postcondition:
        - Elke werknemer verwerkt zijn huidige bestelling.
        - Afgewerkte bestellingen worden gelogd.
        - Nieuwe bestellingen worden toegekend indien mogelijk.
        - De logs worden bijgewerkt en de tijd wordt verhoogd.
        """
        print(f"[System] TICK: Tijd {self.time} start")

        werknemers = []
        self.werknemers.traverseTableValues(werknemers.append)

        werknemersToReset = app.Stack(999)
        for werknemer in werknemers:
            print(werknemer.getName() + " heeft " + str(werknemer.nogTeDoen) + " credits te gaan")
            if werknemer.huidigebestelling is not None:
                werknemer.werk()
                if werknemer.isKlaar():
                    bestelling = werknemer.huidigebestelling
                    self.beschikbaar.push(werknemer)
                    self.afgewerkt.tableInsert((self.afgewerkt.tableLength(), bestelling))
                    #hiertussen moet eigen de log updaten
                    werknemer.reset()
                    werknemersToReset.push(werknemer)
                    print(f"[System] Werknemer {werknemer.getName()} heeft bestelling afgerond")

        while not self.beschikbaar.isEmpty() and not self.bestellingen.isEmpty():
            self.behandelbestellingen()

        self.updateLogs()
        self.clearNieuweBestelingen()
        self.time += 1
        print(f"[System] TICK: Tijd verhoogd naar {self.time}")

    # ----------------------------------------------------------
    # Logging
    # ----------------------------------------------------------
    def log(self):
        """
        Genereert en sluit een HTML-logbestand af waarin de staat van het systeem wordt weergegeven.

        Postcondition:
        - Een HTML-log wordt gegenereerd en opgeslagen in self.logs.
        - De log bevat informatie over tijdstippen, werknemers, bestellingen en stockstatus.
        """
        html_content_deel1 = """<head><style>body{font-family:monospace;color:#0f172a;}table{border-collapse:collapse;}td{border:1px solid #0f172a;white-space:nowrap;padding:0.25em 0.5em;font-size:0.8em;}tr{background-color:#f1f5f9;}th{background-color:#cbd5e1;border:1px solid #0f172a;white-space:nowrap;padding:0.25em 0.5em;font-size:1em;}</style></head>\n<body><table>\n<tr><th rowspan="2">Tijdstip</th><th colspan="""

        html_content_deel2 = """Werknemers</th><th colspan="2">Bestellingen</th><th colspan="7">Stock</th></tr>\n<tr><th>Stack</th>"""

        html_content_deel3 = """<th>Nieuw</th><th>Queue</th><th>wit</th><th>melk</th><th>bruin</th><th>zwart</th><th>honing</th><th>marshmallow</th><th>chilipeper</th></tr>\n"""

        html_einde = """</table></body>"""

        log_filename = "log" + str(self.time) + ".html"

        # berekent hoeveel kolommen er nodig zijn voor het aantal werknemers
        aantalWerknemers = "\"" + str(self.werknemers.tableLength()+1) + "\">"

        # maakt voor elke werknemer een kolom aan met zijn\haar naam
        werknemersString = ""
        keys = []
        self.werknemers.traverseTableKeys(lambda x: keys.append(x))
        for key in keys:
            to_delete = self.werknemers.tableRetrieve(key)[0]
            werknemersString += "<th>" + to_delete.getName() + "</th>"

        # voegt de begin en eind code toe aan de string
        self.logs = html_content_deel1 + aantalWerknemers + html_content_deel2 + werknemersString + html_content_deel3 + self.logs + html_einde

        print(f"[System] Logbestand opgeslagen als {log_filename}")

    def updateLogs(self):
        """
        Voegt een nieuwe rij toe aan het HTML-logbestand met de huidige status van het systeem.

        Postcondition:
        - Een extra rij met info over werknemers, bestellingen en stocks is toegevoegd aan self.logs.
        """

        #controleert of de html file nog niet afgesloten is
        if self.logs.endswith("</table></body>"):
            return

        # maakt een nieuwe rij aan met het tijdstip
        self.logs += "<tr><td>" + str(self.time) + "</td>"

        # voegt de stack van beschikbare werknemers toe
        self.logs += "<td>|"
        stack = ""
        beschikbaarreverse = app.Stack(self.sizeofsq)
        while not self.beschikbaar.isEmpty():
            werknemer = self.beschikbaar.pop()[0]
            beschikbaarreverse.push(werknemer)
            stack += "" + str(werknemer.workload)
        omgekeerd = " ".join(reversed(stack))
        self.logs += omgekeerd
        self.logs += "</td>"

        while not beschikbaarreverse.isEmpty():
            self.beschikbaar.push(beschikbaarreverse.pop()[0])

        # gaat bij elke werknemer langs om te zien of ze aan een bepaalde bestelling bezig zijn en zet het aantal credits dat ze nog moeten doen in de file
        keys = []
        self.werknemers.traverseTableKeys(lambda x: keys.append(x))
        for key in keys:
            to_delete = self.werknemers.tableRetrieve(key)[0]
            if to_delete.nogTeDoen == 0:
                self.logs += "<td></td>"
            else:
                self.logs += "<td>" + str(to_delete.nogTeDoen) + "</td>"

        # voegt de nieuwe bestelling toe aan de logs
        bestellingsstring = "<td>"
        if len(self.nieuwebestellingen) == 0:
            bestellingsstring += "  "
        for bestelling in self.nieuwebestellingen:
            bestellingsstring += str(bestelling.credits) + ", "
        bestellingsstring = bestellingsstring[:-2]
        bestellingsstring += "</td>"
        self.logs += bestellingsstring

        # voegt de queue van bestellingen toe aan de logs
        bestellingsstring = "<td>"
        bestellingen_extra = app.Queue(self.sizeofsq)
        if self.bestellingen.isEmpty():
            bestellingsstring += "  "
        while not self.bestellingen.isEmpty():
            bestellingTussen = self.bestellingen.dequeue()[0]
            bestellingsstring += str(bestellingTussen.getCredits()) + ", "
            bestellingen_extra.enqueue(bestellingTussen)
        bestellingsstring = bestellingsstring[:-2]
        while not bestellingen_extra.isEmpty():
            self.bestellingen.enqueue(bestellingen_extra.dequeue()[0])
        bestellingsstring += "</td>"
        self.logs += bestellingsstring

        # voegt de stocks toe aan de logs
        self.logs += "<td>" + str(self.stocks.getstocks()[0]) + "</td>" # wite chockolade
        self.logs += "<td>" + str(self.stocks.getstocks()[2]) + "</td>" # melk chocolade
        self.logs += "<td>" + str(self.stocks.getstocks()[1]) + "</td>" # bruine chockolade
        self.logs += "<td>" + str(self.stocks.getstocks()[3]) + "</td>" # zwarte chockolade
        self.logs += "<td>" + str(self.stocks.getstocks()[4]) + "</td>" # honing
        self.logs += "<td>" + str(self.stocks.getstocks()[5]) + "</td>" # marshmellows
        self.logs += "<td>" + str(self.stocks.getstocks()[6]) + "</td>" # chilipepers

        self.logs += "</tr>\n" # einde van de rij
        print(f"[System] Logs bijgewerkt voor tijdstip {self.time}")
