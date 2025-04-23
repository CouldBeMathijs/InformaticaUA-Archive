"""
input.py: Dit programma leest een invoerbestand dat een constante menselijke vorm heeft en zet het om in instructies.

Precondition:
- Een geldig tekstbestand bevindt zich op een opgegeven locatie.
- Het bestand bevat gestructureerde gegevens in menselijke vorm.

Postcondition:
- De inhoud van het bestand is gelezen en beschikbaar voor verdere functies op te roepen.

Auteur: Groep 10
Datum: 25-2-2025
"""

# ----------------------------------------------------------
# Class: Input
# ----------------------------------------------------------
class Input:
    """
    Deze klasse beheert het inlezen en verwerken van instructies uit een stringinput.

    Parameters:
    - content (str): De (tekst)inhoud die moet worden ingelezen en verwerkt.
    - system (object): Een systeemobject met onder andere een stocks-attribuut.

    Precondition:
    - content is niet None en bevat de gegevens die verwerkt moeten worden.
    - system is een geldig systeemobject met een 'stocks'-attribuut.

    Postcondition:
    - De content is opgeschoond en instructies kunnen verwerkt worden.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, content, system):
        """
        Initialiseer de Input-klasse en verwerk de aangeleverde content.

        Parameters:
        - content (str): De tekst die moet worden ingelezen en opgeschoond.
        - system (object): Het systeemobject met minimaal een 'stocks'-attribuut.

        Precondition:
        - content bevat de te verwerken instructies.
        - system is een object dat de benodigde methoden/attributen implementeert.

        Postcondition:
        - self.content bevat de opgeschoonde lijst met regels.
        """
        self.system = system
        self.stocks = system.stocks
        self.content = content.splitlines()

        self.cleanFileContent()

    # ----------------------------------------------------------
    # Bestandsverwerking: opschonen
    # ----------------------------------------------------------
    def cleanFileContent(self):
        """
        Neemt een lijst met regels en verwerkt deze:
        1. Verwijdert lege regels
        2. Verwijdert commentaarregels
        3. Trimt witruimte aan het begin en einde van elke regel
        4. Retourneert een opgeschoonde lijst zonder overbodige tekens

        Precondition:
        - self.content is niet None.

        Postcondition:
        - self.content bevat geen lege regels en geen commentaarregels.
        """
        if self.content is None:
            print("Fout: Geen inhoud om te verwerken.")
            return
        
        cleanedLines = []
        
        for line in self.content:
            strippedLine = line.strip()
            
            if not strippedLine:
                continue
            
            if strippedLine.startswith("#"):
                continue
            
            cleanedLines.append(strippedLine)
        
        self.content =  cleanedLines

    # ----------------------------------------------------------
    # Instructieverwerking
    # ----------------------------------------------------------
    def processInitInstructions(self):
        """
        Verwerkt de instructies één voor één van boven naar beneden.
        Controleert of 'init' en 'start' aanwezig zijn en verwerkt overige instructies.

        Precondition:
        - self.content bevat instructies in de juiste vorm.

        Postcondition:
        - De instructies 'init' en 'start' worden gecontroleerd.
        - Overige instructies worden door hun respectievelijke handlers afgehandeld.
        """

        if not self.content:
            print("Fout: Geen inhoud om te verwerken.")
            return

        if "init" not in self.content or "start" not in self.content:
            print("Fout: 'init' of 'start' instructie ontbreekt.")
            return
        
        commandMap = {
            "init": self.handleInit,
            "shot": self.handleShot,
            "marshmallow": self.handleTopping,
            "honing": self.handleTopping,
            "chilipeper": self.handleTopping,
            "gebruiker": self.handleGebruiker,
            "werknemer": self.handleWerknemer,              
            "start": self.handleStart
        }

        for line in self.content[:]:
            parts = line.split()
            command = parts[0]
            if command in commandMap:
                commandMap[command](parts)
                self.content.remove(line)
        
    def handleInit(self, parts):
        """
        Verwerk de 'init' instructie.

        Parameters:
        - parts[0] == 'init'

        Postcondition:
        - Een melding wordt geprint dat de initialisatie start.
        """
        print("<---Starten met het initialiseren van instructies--->")

    def handleShot(self, parts):
        """
        Verwerk een shot-instructie.

        - parts[0]: "shot" (klasse)
        - parts[1]: type
        - parts[2]: aantal
        - parts[3]: jaar
        - parts[4]: maand
        - parts[5]: dag

        Precondition:
        - parts bevat minstens 6 elementen.

        Postcondition:
        - Functie om shot toe te voegen wordt aangeroepen met de juiste parameters.
        """
        if len(parts) < 6:
            print("Fout: Onvoldoende gegevens voor shot!")
            return

        klasse = parts[0]
        type = parts[1]
        aantal = int(parts[2])
        jaar = parts[3]
        maand = parts[4]
        dag = parts[5]

        self.stocks.addStock(klasse, type, aantal, jaar, maand, dag)

    def handleTopping(self, parts):
        """
        Verwerk de toppings-instructie.

        - parts[0]: "topping" (klasse)
        - parts[1]: aantal
        - parts[2]: jaar
        - parts[3]: maand
        - parts[4]: dag

        Precondition:
        - parts bevat minstens 5 elementen.

        Postcondition:
        - Functie om toppings toe te voegen wordt aangeroepen met de juiste parameters.
        """
        if len(parts) < 5:
            print("Fout: Onvoldoende gegevens voor marshmallow!")
            return

        klasse = parts[0]
        aantal = int(parts[1])
        jaar = parts[2]
        maand = parts[3]
        dag = parts[4]

        self.stocks.addStock(klasse, None, aantal, jaar, maand, dag)

    def handleGebruiker(self, parts):
        """
        Verwerk een gebruiker-instructie.
        
        - parts[0]: "gebruiker" (commando)
        - parts[1]: voornaam
        - parts[2]: achternaam
        - parts[3]: emailadres

        Precondition:
        - parts bevat minstens 4 elementen.

        Postcondition:
        - Functie om gebruiker toe te voegen wordt aangeroepen met de juiste parameters.
        """
        if len(parts) < 4:
            print("Fout: Onvoldoende gegevens voor gebruiker!")
            return
        
        voornaam = parts[1]
        achternaam = parts[2]
        email = parts[3]

        self.system.addGebruiker(voornaam, achternaam, email)

    def handleWerknemer(self, parts):
        """
        Verwerk een werknemer-instructie.

        - parts[0]: "werknemer" (commando)
        - parts[1]: voornaam
        - parts[2]: achternaam
        - parts[3]: werknemer ID

        Precondition:
        - parts bevat minstens 4 elementen.

        Postcondition:
        - Functie om werknemer toe te voegen wordt aangeroepen met de juiste parameters.
        """
        if len(parts) < 4:
            print("Fout: Onvoldoende gegevens voor werknemer!")
            return

        voornaam = parts[1]
        achternaam = parts[2]
        werklast = parts[3]

        self.system.addWerknemer(voornaam, achternaam, int(werklast))

    def handleStart(self, parts):
        """
        Verwerkt de 'start' instructie.

        Parameters:
        - parts[0]: "start"

        Precondition:
        - parts[0] == 'start'.

        Postcondition:
        - Een melding wordt geprint dat de instructies zijn geïnitialiseerd.
        """
        print("<----------Instructies zijn geïnitialiseerd!--------->")
       

    # ----------------------------------------------------------
    # Timestamp instructies
    # ----------------------------------------------------------
    def readTimestampInstruction(self, timestampr):
        """
        Doorzoekt self.content naar een instructie met een specifiek timestamp
        en verwerkt deze indien aanwezig.

        Parameters:
        - timestampr (int): De timestamp die gezocht wordt in de eerste kolom.

        Return:
        - bool: True als er nog inhoud in self.content overblijft, anders False.

        Precondition:
        - self.content bevat geldige instructies, waarvan de eerste kolom
          optioneel een timestamp bevat.

        Postcondition:
        - De bijbehorende instructie (indien gevonden) wordt afgehandeld.
        - Foutmelding indien er geen geldige instructie is.
        """
        if not self.content or len(self.content) == 0:
            print("Fout: Geen inhoud om te verwerken.")
            return False, False

        commandMap = {
            "bestel": self.handleBestel,
            "stock": self.handleStock,
            "log": self.handleLog
        }

        logFunctionCalled = False

        for line in self.content[:]:
            parts = line.split()
            if not parts:
                continue

            timestamp = parts[0]

            if timestamp == str(timestampr):
                if len(parts) < 2:
                    print("Fout: Ongeldige instructie!")
                    continue

                command = parts[1]
                if command in commandMap:
                    if command == "log":
                        logFunctionCalled = True
                    commandMap[command](parts)

                self.content.remove(line)

        return logFunctionCalled, bool(self.content)

    def handleBestel(self, parts):
        """
        Verwerk een bestel-instructie.

        - parts[0]: tijdstamp
        - parts[1]: "bestel" (commando)
        - parts[2]: gebruiker email
        - parts[3..n-5]: ingrediënten
        - parts[n-4]: jaar
        - parts[n-3]: maand
        - parts[n-2]: dag
        - parts[n-1]: uur
        - parts[n]: minuut

        Precondition:
        - parts bevat minstens 9 elementen.

        Postcondition:
        - Er wordt een melding geprint met bestelinformatie.
        """
        if len(parts) < 9:
            print("Fout: Onvoldoende gegevens voor bestel!")
            return
        
        n = len(parts) - 1
        gebruikerEmail = parts[2]
        ingredienten = parts[3:n-4]
        jaar = parts[n-4]
        maand = parts[n-3]
        dag = parts[n-2]
        uur = parts[n-1]
        minuut = parts[n]

        self.system.bestel(gebruikerEmail, ingredienten, jaar, maand, dag, uur, minuut)

    def handleStock(self, parts):
        """
        Verwerk een stock-instructie.

        - parts[0]: tijdstamp
        - parts[1]: "stock"  (commando)
        - parts[2]: type
        (optioneel) parts[3]: variant (als type == 'shot')
        - parts[n-3]: aantal
        - parts[n-2]: jaar
        - parts[n-1]: maand
        - parts[n]: dag

        Precondition:
        - parts bevat minstens 7 elementen.

        Postcondition:
        - Er wordt een melding geprint met voorraadinformatie.
        """
        if len(parts) < 7:
            print("Fout: Onvoldoende gegevens voor stock!")
            return

        klasse = parts[2]

        type = None
        if klasse == "shot":
            type = parts[3]

        n = len(parts) - 1
        aantal = int(parts[n-3])
        jaar = parts[n-2]
        maand = parts[n-1]
        dag = parts[n]

        self.stocks.addStock(klasse, type, aantal, jaar, maand, dag)

    def handleLog(self, parts):
        """
        Verwerk een log-instructie.

        - parts[0]: tijdstamp
        - parts[1]: "log"  (commando)

        Precondition:
        - parts bevat minstens 2 elementen.

        Postcondition:
        - Er wordt een melding geprint dat de log wordt verwerkt.
        """
        if len(parts) < 2:
            print("Fout: Onvoldoende gegevens voor log!")
            return

        self.system.tick()
        print("FINAL LOG")
        self.system.log()