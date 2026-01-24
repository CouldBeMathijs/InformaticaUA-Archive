"""
Marshmallow.py: Klasse voor het representeren van een marshmallow.

Precondition:
- Een geldig ID moet worden meegegeven.
- Een correcte vervaldatum moet worden opgegeven.

Postcondition:
- Een Marshmallow-object wordt gecreëerd met een standaardprijs van 0.75.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Marshmallow
# ----------------------------------------------------------
class Marshmallow:
    """
    Precondition:
    - id moet een unieke identifier zijn.
    - vervaldatum moet een geldige datumstring zijn.

    Postcondition:
    - Een Marshmallow-object wordt aangemaakt met een ID, vervaldatum en standaardprijs.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, vervaldatum):
        """
        Initialiseert een Marshmallow-object.

        Parameters:
        - id: Unieke identificatie voor de marshmallow.
        - vervaldatum: Datum waarop de marshmallow vervalt.

        Postcondition:
        - Het Marshmallow-object is correct geïnitialiseerd met een standaardprijs van 0.75.
        """
        self.id = id
        self.vervaldatum = vervaldatum
        self.prijs = 0.75
