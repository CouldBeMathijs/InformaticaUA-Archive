"""
Bestelling.py: Klasse voor het representeren van een bestelling.

Precondition:
- Een geldig ID moet worden meegegeven.
- `gebruikersid` moet verwijzen naar een bestaande gebruiker.
- `timestamp` moet een geldig tijdstip zijn.
- `chocolademelkid` moet verwijzen naar een bestaande chocolademelk.
- `toppingsarray` moet een lijst van toppings bevatten.

Postcondition:
- Een Bestelling-object wordt gecreëerd met een standaardwaarde van `isaf` als False
  en een berekende credits-waarde.

Auteur: Groep 10
Datum: 25-02-2025
"""

# ----------------------------------------------------------
# Klasse Bestelling
# ----------------------------------------------------------
class Bestelling:
    """
    Klasse voor het aanmaken van een bestelling.

    Precondition:
    - id is een geldige, unieke identifier.
    - gebruikersid verwijst naar een bestaande gebruiker.
    - timestamp is een geldig tijdstip.
    - chocolademelkid verwijst naar een bestaande chocolademelk.
    - toppingsarray is een lijst met geldige toppings.

    Postcondition:
    - Een Bestelling-object wordt aangemaakt met `isaf` = False,
      geen toegewezen afwerker, en een berekende creditswaarde.
    """

    # ----------------------------------------------------------
    # Initialisatie
    # ----------------------------------------------------------
    def __init__(self, id, gebruikersid, timestamp, chocolademelkid, toppingsarray):
        """
        Initialiseert een Bestelling-object.

        Parameters:
        - id: Unieke identificatie voor de bestelling.
        - gebruikersid: ID van de gebruiker die de bestelling plaatst.
        - timestamp: Tijdstip waarop de bestelling geplaatst is.
        - chocolademelkid: ID van de gekozen chocolademelk.
        - toppingsarray: Lijst met toppings voor de bestelling.

        Postcondition:
        - Een Bestelling-object wordt aangemaakt met `isaf` = False,
          geen toegewezen afwerker en een berekende creditswaarde.
        """
        self.isaf = False            # Bestelling is nog niet afgewerkt
        self.afwerker = None        # Geen toegewezen afwerker

        self.id = id
        self.gebruikersid = gebruikersid
        self.timestamp = timestamp
        self.chocolademelkid = chocolademelkid
        self.credits = 5 + len(toppingsarray)

    def getCredits(self):
        """
        Retourneert het aantal credits dat deze bestelling waard is.

        Return:
        - int: het aantal credits.
        """
        return self.credits
