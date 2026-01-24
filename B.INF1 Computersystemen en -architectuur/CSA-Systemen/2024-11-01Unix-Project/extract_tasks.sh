#!/bin/bash

# Oefening 7
organize_dirs() {
    cd "$1"
    ls
    # Alle bestanden moeten op 1 niveau komen te liggen, 2 niveau's onder
   	for student_dir in *; do
		# Verplaats zonder de dirname werkte het niet omdat BASH dacht dan ook files naar submappen van bestanden probeerde schrijven?
        
        # Verplaats alle bestanden naar het juiste niveau
        find "$student_dir" -type f -exec mv {} "$student_dir" \;
        
        # Verwijder alle submappen in de mappen van de studenten 
        find "$student_dir" -type d -delete -empty
		ls "$student_dir"
        # Verwijder alle bestanden die niet de juiste uitgang hebben
        for file in $student_dir; do
        	find -not -name "*$EXTENSION" -not -name late_inzending -delete
        done
    done
}


# Debugger, om snel aan en uit te kunnen zetten
# set -x
# Oefening1
BESTAND="$1"
EXTENSION="$2"
EVALUATIONSCRIPT="$3"
export EXTENSION="$2"
STARTMAP=$(pwd)
# Kijk of de bestandsnaam het juiste formaat heeft (BASH_REMATCH vond ik online en leek juist te zijn wat nodig is om dit stuk van de code veel eenvoudiger te maken, ik  deed het origineel met 3 regex formules)
# Onderstaande regex uitgelegd:
# - VAK alles dat tussen "assignment_UA_" en de volgende "_" ligt
# - TITEL tussen die "_" en de volgende "_"
# - DEADLINE de datum op het eind (YYYY-MM-DD)
if [[ "$BESTAND" =~ assignment_UA_([^_]+)_([^_]+)_([0-9]{4}-[0-9]{2}-[0-9]{2})\.tgz ]]; then
    VAK="${BASH_REMATCH[1]}"
    TITEL="${BASH_REMATCH[2]}"
    DEADLINE="${BASH_REMATCH[3]}"
else
    echo "Het bestand heeft niet de juiste naamvorm of is niet van het juiste type."
    exit 1
fi
#De juiste folders aanmaken
OPSLAGPLAATS="$VAK/$TITEL"
if [ ! -d "$OPSLAGPLAATS" ]; then
    echo "Map aanmaken: $OPSLAGPLAATS"
    mkdir -p "$OPSLAGPLAATS"
else
    echo "Map $OPSLAGPLAATS bestaat al"
fi
#De bestanden uitpakken naar de juiste map, zonder het hoogste niveau in de folderstructuur mee uit te pakken
tar -xvf "$BESTAND" --directory="$OPSLAGPLAATS" --strip-components=1

echo "Opslagplaats: $OPSLAGPLAATS"

# Oefening 2: extra testen oefening 1
# Oefening 3
cd "$OPSLAGPLAATS"
for file in *; do
    if [ -f "$file" ]; then
    	# Verwijder alle spaties
        file2="${file// /}"
        if [ ! "$file" == "$file2" ]; then  
        	mv "$file" "$file2"  
        fi
    fi
done
for SUBBESTAND in *; do
    # Vereenvoudigde regex voor Bash:
    # - TITEL: elke reeks tekens tot aan de eerste _
    # - VOORNAAM: elke reeks tekens tot aan de punt
    # - ACHTERNAAM: elke reeks tekens, inclusief spaties, tot aan de volgende vaste tekst ".s.ua_poging_"
    # - UPLOADDATUM: datum in het formaat YYYY-MM-DD

	if [[ "$SUBBESTAND" =~ ^(.+)_([^\._]+)\.([[:alnum:][:space:]]+)\.s\.ua_poging_([0-9]{4}-[0-9]{2}-[0-9]{2})\.tgz$ ]]; then
    	TITEL="${BASH_REMATCH[1]}"
    	VOORNAAM="${BASH_REMATCH[2]}"
    	ACHTERNAAM="${BASH_REMATCH[3]}"
	    UPLOADDATUM="${BASH_REMATCH[4]}"
	    echo "$SUBBESTAND correct geformateerd."
	else
        echo "Een subbestand '$SUBBESTAND' heeft niet de juiste naamvorm of is niet van het juiste type, verdergaan naar volgend bestand..."
	fi
	# De juiste folders aanmaken
	mkdir -p "$ACHTERNAAM.$VOORNAAM"
	# Pak de bestanden uit
	echo $(pwd)
	tar -xvf "$SUBBESTAND" --directory="$ACHTERNAAM.$VOORNAAM" --strip-components=1
	rm "$SUBBESTAND"
	# Oefening 5
	# Data omzetten naar UNIX-time (aantal seconden sinds middernacht van 1 januari 1970)
	UNIXDEADLINE=$(date -d "$DEADLINE" +%s)
	UNIXUPLOADDATUM=$(date -d "$UPLOADDATUM" +%s)
	if [ "$UNIXUPLOADDATUM" -gt "$UNIXDEADLINE" ]; then
		touch "$ACHTERNAAM.$VOORNAAM/late_inzending"
		echo "Een bestand van $VOORNAAM $ACHTERNAAM werd te laat ingezonden."
	fi
done
echo "$(pwd)"
organize_dirs "$OPSLAGPLAATS"

for subfolder in *; do
	if [ ! -d $subfolder ]; then
		continue
	fi
	echo "$EVALUATIONSCRIPT wordt uitgevoerd op de map $subfolder"
	source "$EVALUATIONSCRIPT" "$subfolder"
done
cd "$STARTMAP"
