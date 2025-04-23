"""
Chilipeper.py: Klasse voor het representeren van een chilipeper.

Precondition:
- Een geldig ID moet worden meegegeven.
- Een correcte vervaldatum moet worden opgegeven.

Postcondition:
- Een chilipeper-object wordt gecreëerd met een standaardprijs van 0.25.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Chilipeper
# ----------------------------------------------------------
class Chilipeper:
    """
    Precondition:
    - id moet een unieke identifier zijn.
    - vervaldatum moet een geldige datumstring zijn.

    Postcondition:
    - Een Chilipeper-object wordt aangemaakt met een ID, vervaldatum en standaardprijs.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, vervaldatum):
        """
        Initialiseert een Chilipeper-object.

        Parameters:
        - id: Unieke identificatie voor de chilipeper.
        - vervaldatum: Datum waarop de chilipeper vervalt.

        Postcondition:
        - Het Chilipeper-object is correct geïnitialiseerd met een standaardprijs van 0.25.
        """
        self.id = id
        self.vervaldatum = vervaldatum
        self.prijs = 0.25
