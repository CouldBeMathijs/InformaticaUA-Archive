# GAS TO groep 10


## Code Commentaar & Layout Richtlijnen

### Doel
Deze richtlijnen beschrijven de structuur voor commentaar en layout in onze code. Het doel is om de leesbaarheid en consistentie te verbeteren zonder de complexiteit onnodig te verhogen.

### Bestandsheader
Elke broncodefile moet een header bevatten met:
- Een korte beschrijving van de functionaliteit.
- Precondition en Postcondition (indien van toepassing).
- Eventueel auteur en datum.

**Template:**
```python
"""
<Bestandsnaam>: Beschrijving van de functionaliteit.

Precondition:
- Voorbeeld: Een geldig tekstbestand moet aanwezig zijn.

Postcondition:
- Voorbeeld: De inhoud wordt ingelezen en beschikbaar gesteld.

Auteur: [Jouw Naam]
Datum: [Datum]
"""
```

### Secties en Layout
Gebruik duidelijke sectie-indelingen met horizontale lijnen of commentaarblokken om de code in logische delen te verdelen.

**Template:**
```python
# ----------------------------------------------------------
# Sectie Naam: Beschrijving van de sectie
# ----------------------------------------------------------
```

### Functie Commentaar
Elke functie dient een duidelijke docstring te hebben met:
- Een korte beschrijving van de functionaliteit.
- Overzicht van parameters en hun betekenis.
- Beschrijving van de return-waarde (indien aanwezig).
- Vermelding van pre- en postcondities (indien van toepassing).

**Template:**
```python
def functieNaam(param1, param2):
    """
    Beschrijving: Wat doet de functie?

    Precondition:
    - Voorbeeld: param1 moet een positief getal zijn.

    Postcondition:
    - Voorbeeld: Geeft het resultaat van de berekening terug.
    """
```
&iW
