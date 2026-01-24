from quetzal.System import *
from quetzal.input import *

from gas.student4.MyStack import MyStack as Stack
from gas.student3.MyQueue import MyQueue as Queue
from gas.student2.Bst import BSTTable as Table
#from gas.student1.TwoThreeTree import TwoThreeTreeTable as Table
from quetzal.Bestelling import *
from quetzal.Gebruiker import *
from quetzal.Stocks import *
from quetzal.Werknemer import *

from quetzal.Chocoladeshot import *
from quetzal.Honing import *
from quetzal.Chilipeper import *
from quetzal.Marshmallow import *

from quetzal.Chocolademelk import *

import time

class App:
    """
    !DE CODE IN DEZE KLASSE MOET GEÏMPLEMENTEERD WORDEN!

    Deze klasse bestuurt het systeem en dient als het ingangspunt voor het uitvoeren van een script.
    De App.run methode zal een script uitvoeren en de logs teruggeven.
    De App.get_internals methode zal de datastructuren van het systeem teruggeven.

    Noot: App.run zal hoogstens 1 keer opgeroepen worden per instantie van App. Als er nog een script uitgevoerd moet worden, zal er een nieuwe instantie van App aangemaakt worden.
    Noot: App.get_internals moet altijd dezelfde objecten teruggeven (zowel voor als na het uitvoeren van een script)
    """

    def __init__(self):
        """
        Initialiseeert het systeem.

        Preconditie: geen
        Postconditie: App.run en App.get_internals kunnen aangeroepen worden.
        Postconditie: App.get_internals geeft nu altijd dezelfde objecten terug
        """
        self.quetzal = System()
        self.input = None

    def run(self, script):
        """
        Voert een script uit en geeft de logs terug.

        Preconditie: het systeem is geïnitialiseerd (App.__init__ is aangeroepen)
        Preconditie: dit is de eerste keer dat een script wordt uitgevoerd
        Preconditie: script is een string van het juiste formaat
        Postconditie: de logs worden teruggegeven
            voor elke log instructie in het script bevat de output een entry met het tijdstip en de log
        """
        self.input = Input(script, self.quetzal)
        self.input.processInitInstructions()

        instructions_left = True
        while instructions_left:
            # Blijft herhalen tot er geen instructies meer zijn.
            isLog, instructions_left = self.input.readTimestampInstruction(self.quetzal.time) # Excecutes the instructions and updates the log
            if not isLog:
                self.quetzal.tick()
            else:
                break

        with open(f"log.html", "w") as file:
            file.write(self.quetzal.logs)

        # Hier hoort de main loop over alle tijdstippen te komen, dit script moet de logs teruggeven
        return { self.quetzal.time-1 : self.quetzal.logs}



    def get_internals(self):
        """
        Geeft (een selectie van) de interne datastructuren van het systeem terug.

        Preconditie: het systeem is geïnitialiseerd (App.__init__ is aangeroepen)
        Postconditie: de volgende interne datastructuren worden teruggegeven
            gebruiker_tabel: de tabel van gebruikers
            werknemer_tabel: de tabel van werknemers
            bestelling_tabel: de tabel van afgewerkte bestellingen
            werknemer_stack: de stack van werknemers
            bestelling_queue: de queue van wachtende bestellingen
            honing_stock: de tabel van de honing stock
            chilipeper_stock: de tabel van de chilipeper stock
            marshmallow_stock: de tabel van de marshmallow stock
        Postconditie: geeft altijd dezelfde objecten terug (zowel voor als na het uitvoeren van een script)
        """

        return {
            "gebruiker_tabel": self.quetzal.gebruikers,
            "werknemer_tabel": self.quetzal.werknemers,
            "bestelling_tabel": self.quetzal.afgewerkt,
            "werknemer_stack": self.quetzal.beschikbaar,
            "bestelling_queue": self.quetzal.bestellingen,
            "honing_stock": self.quetzal.stocks.honingporties,
            "chilipeper_stock": self.quetzal.stocks.chilipeperporties,
            "marshmallow_stock": self.quetzal.stocks.marshmallows,
        }
