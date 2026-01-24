"""
Werknemer.py: Klasse voor het representeren van een werknemer.

Precondition:
- Een geldig ID moet worden meegegeven.
- Voornaam en achternaam moeten strings zijn.
- Workload moet een numerieke waarde zijn.

Postcondition:
- Een Werknemer-object wordt gecreëerd met een standaardwaarde van `huidigebestelling` als None.

Auteur: Groep 10
Datum: 21-02-2025
"""

# ----------------------------------------------------------
# Klasse Werknemer
# ----------------------------------------------------------
class Werknemer:
    """
    Precondition:
    - id moet een unieke identifier zijn.
    - voornaam en achternaam moeten strings zijn.
    - workload moet een numerieke waarde zijn.

    Postcondition:
    - Een Werknemer-object wordt aangemaakt met een standaardwaarde van `huidigebestelling` = None
      en een workload die aangeeft hoeveel credits de werknemer per tijdeenheid aankan.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, voornaam, achternaam, workload):
        """
        Initialiseert een Werknemer-object.

        Parameters:
        - id: Unieke identificatie voor de werknemer.
        - voornaam: Voornaam van de werknemer.
        - achternaam: Achternaam van de werknemer.
        - workload: Hoeveel credits deze werknemer per tijdeenheid aankan.

        Postcondition:
        - Een Werknemer-object is aangemaakt met de opgegeven id, voornaam, achternaam, workload,
          een nogTeDoen-waarde van 0 en geen actieve bestelling.
        """
        self.id = id
        self.voornaam = voornaam
        self.achternaam = achternaam
        self.workload = workload
        self.nogTeDoen = 0
        self.huidigebestelling = None

    # ----------------------------------------------------------
    # Methoden
    # ----------------------------------------------------------
    def reset(self):
        self.nogTeDoen = 0
        self.huidigebestelling = None

    def behandel(self, bestelling):
        self.huidigebestelling = bestelling
        self.nogTeDoen = bestelling.credits

    def werk(self):
        self.nogTeDoen -= self.workload

    def isKlaar(self):
        if self.nogTeDoen <= 0:
            return True
        else:
            return False

    def getName(self):
        """
        Haalt de volledige naam van de werknemer op.

        Postcondition:
        - Retourneert de volledige naam als een string.

        Return:
        - De volledige naam van de werknemer.
        """
        return self.voornaam + " " + self.achternaam