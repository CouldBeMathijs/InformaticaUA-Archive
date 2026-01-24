"""
Gebruiker.py: Klasse voor het representeren van een gebruiker.

Precondition:
- Een geldig ID moet worden meegegeven.
- Voornaam, achternaam en email moeten strings zijn.

Postcondition:
- Een Gebruiker-object wordt gecreëerd met de opgegeven gegevens.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Gebruiker
# ----------------------------------------------------------
class Gebruiker:
    """
    Precondition:
    - id moet een unieke identifier zijn.
    - voornaam en achternaam moeten strings zijn.
    - email moet een geldige e-mailstring zijn.

    Postcondition:
    - Een Gebruiker-object wordt gecreëerd met een ID, voornaam, achternaam en email.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, voornaam, achternaam, email):
        """
        Initialiseert een Gebruiker-object.

        Parameters:
        - id: Unieke identificatie voor de gebruiker.
        - voornaam: Voornaam van de gebruiker.
        - achternaam: Achternaam van de gebruiker.
        - email: E-mailadres van de gebruiker.

        Postcondition:
        - Het Gebruiker-object is correct geïnitialiseerd met de opgegeven gegevens.
        """
        self.id = id
        self.voornaam = voornaam
        self.achternaam = achternaam
        self.email = email

    # ----------------------------------------------------------
    # Methoden
    # ----------------------------------------------------------
    def getName(self):
        """
        Haalt de volledige naam van de gebruiker op.

        Postcondition:
        - Retourneert de volledige naam als een string.

        Return:
        - De volledige naam van de gebruiker.
        """
        return self.voornaam + " " + self.achternaam
