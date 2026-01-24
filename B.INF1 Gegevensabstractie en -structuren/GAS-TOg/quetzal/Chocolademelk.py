"""
Chocolademelk.py: Klasse voor het representeren van chocolademelk.

Precondition:
- Een geldig ID moet worden meegegeven.

Postcondition:
- Een chocolademelk-object wordt gecreëerd met een standaardprijs van 2.0.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Chocolademelk
# ----------------------------------------------------------
class Chocolademelk:
    """
    Precondition:
    - id moet een unieke identifier zijn.

    Postcondition:
    - Een Chocolademelk-object wordt aangemaakt met een ID en standaardprijs.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id):
        """
        Initialiseert een Chocolademelk-object.

        Parameters:
        - id: Unieke identificatie voor de chocolademelk.

        Postcondition:
        - Het Chocolademelk-object is correct geïnitialiseerd met een standaardprijs van 2.0.
        """
        self.id = id
        self.prijs = 2.0
