"""
Chocoladeshot.py: Klasse voor het representeren van een chocoladeshot.

Precondition:
- Een geldig type chocoladeshot moet worden meegegeven (wit, bruin, puur of zwart).
- Een uniek ID moet worden toegekend.

Postcondition:
- Een chocoladeshot-object wordt gecreëerd met een standaardprijs van 1.0.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Chocoladeshot
# ----------------------------------------------------------
class Chocoladeshot:
    """
    Precondition:
    - type is een geldige string (wit, bruin, puur of zwart).
    - id is een unieke identifier.

    Postcondition:
    - Een Chocoladeshot-object wordt aangemaakt met een type, id en een standaardprijs van 1.0.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, type, vervaldatum):
        """
        Initialiseert een Chocoladeshot-object.

        Parameters:
        - id: Unieke identificatie voor de chocoladeshot.
        - type: Type chocolade (wit, bruin, puur, zwart).
        - vervaldatum: Datum waarop de chocoladeshot vervalt.

        Postcondition:
        - Het Chocoladeshot-object is correct geïnitialiseerd met een standaardprijs van 1.0.
        """
        self.id = id
        self.type = type
        self.vervaldatum = vervaldatum
        self.prijs = 1.0
