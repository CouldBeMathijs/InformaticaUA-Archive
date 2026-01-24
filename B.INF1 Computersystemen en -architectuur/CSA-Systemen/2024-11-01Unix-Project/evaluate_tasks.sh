#!/bin/bash

# Verwachte mapstructuur
STUDENT_FOLDER="$1"
OUTPUT_DIR="$1/../Oplossingen"

# Controleer of er precies één argument wordt meegegeven (de folder met student submission)
if [ "$#" -ne 1 ]; then
  echo "Gebruik: $0 folder-with-student-submission"
  exit 1
fi

# Controleer of de directory bestaat
if [ ! -d "$STUDENT_FOLDER" ]; then
  echo "Map '$STUDENT_FOLDER' bestaat niet."
  exit 1
fi

if [ ! -d "$OUTPUT_DIR" ]; then
	echo "Map $OUTPUT_DIR, bestaat niet, aanmaken"
	mkdir -p "$OUTPUT_DIR"
fi
# Haal de achternaam en voornaam uit de $STUDENT_FOLDER
IFS='.' read -r ACHTERNAAM VOORNAAM <<< "$(basename "$STUDENT_FOLDER")"

# Loop door elk Python-bestand
for py_file in "$STUDENT_FOLDER"/*.py; do
  FILENAME=$(basename "$py_file" .py)

  # Bestandsnamen voor output en errors
  OUTPUT_FILE="$OUTPUT_DIR/${ACHTERNAAM}.${VOORNAAM}_${FILENAME}_output.txt"
  ERROR_FILE="$OUTPUT_DIR/${ACHTERNAAM}.${VOORNAAM}_${FILENAME}_error.txt"

  # Voer het Python-bestand uit en sla de output en fouten op
  /bin/python3 "$py_file" >"$OUTPUT_FILE" 2>"$ERROR_FILE"
done

echo "Evaluatie voltooid. Output en errors zijn opgeslagen in $OUTPUT_DIR."
