#!/bin/bash

#Oefening1
BESTAND=$1
# Kijk of de bestandsnaam het juiste formaat heeft (BASH_REMATCH vond ik online en leek juist te zijn wat nodig is om dit stuk van de code veel eenvoudiger te maken, ik  deed het origineel met 3 regex formules)
if [[ "$BESTAND" =~ assignment_UA_([^_]+)_([^_]+)_([0-9]{4}-[0-9]{2}-[0-9]{2})\.tgz ]]; then
    VAK="${BASH_REMATCH[1]}"
    TITEL="${BASH_REMATCH[2]}"
    DEADLINE="${BASH_REMATCH[3]}"
else
    echo "Het bestand heeft niet de juiste naamvorm of is niet van het juiste type."
    exit 1
fi
#De juiste folders aanmaken
OPSLAGPLAATS="$VAK"/"$TITEL"
if [ ! -d "$OPSLAGPLAATS" ]; then
    echo "Map aanmaken: $OPSLAGPLAATS"
    mkdir -p "$OPSLAGPLAATS"
else
    echo "Map $OPSLAGPLAATS bestaat al"
fi
#De bestanden uitpakken naar de juiste map, zonder het hoogste niveau in de folderstructuur mee uit te pakken
tar -xvf "$BESTAND" --directory="$OPSLAGPLAATS" --strip-components=1
# Ga naar de juiste plaats, als het mislukt, stop
cd "$OPSLAGPLAATS" || exit 1
# Onderstaande code bleek niet gevraagd te zijn, ook al dacht ik van wel, en het heeft me veel debugtijd gekost om uit te vogelen dat ik te véél aan het doen was, dus laat ik de code er in staan als aandenken
# Bestanden hernoemen zodat er geen spaties inzitten
# for file in *; do
#     if [ -f "$file" ]; then
#     	# Verander alle spaties in underscores
#         file2="${file// /_}"
#         if [ ! "$file" == "$file2" ]; then  
#         	mv "$file" "$file2"  
#         fi
#     fi
# done
echo "Opslagplaats: $OPSLAGPLAATS"
cd ../..
