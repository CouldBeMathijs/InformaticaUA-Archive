"""
Honing.py: Klasse voor het representeren van honing.

Precondition:
- Een geldig ID moet worden meegegeven.
- Een correcte vervaldatum moet worden opgegeven.

Postcondition:
- Een Honing-object wordt gecreëerd met een standaardprijs van 0.5.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Honing
# ----------------------------------------------------------
class Honing:
    """
    Klasse voor het representeren van honingobjecten in het systeem.

    Precondition:
    - id moet een unieke identifier zijn.
    - vervaldatum moet een geldige datumstring zijn.

    Postcondition:
    - Een Honing-object wordt aangemaakt met een ID, vervaldatum en standaardprijs.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, vervaldatum):
        """
        Initialiseert een Honing-object.

        Parameters:
        - id: Unieke identificatie voor de honing.
        - vervaldatum: Datum waarop de honing vervalt.

        Postcondition:
        - Het Honing-object is correct geïnitialiseerd met een standaardprijs van 0.5.
        """
        self.id = id
        self.vervaldatum = vervaldatum
        self.prijs = 0.5
