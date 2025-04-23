"""
!DE CODE IN DIT BESTAND MOET NIET (MAAR MAG WEL) AANGEPAST WORDEN!

Deze code roept jullie systeem aan met het inputbestand dat de gebruiker meegeeft.
De gegenereerde logs worden weggeschreven naar een output directory met dezelfde naam als het inputbestand.

`python main.py tests/input/voorbeeld1.txt` zal een log schrijven naar `tests/input/voorbeeld1/log4.html`

Zie quetzal/app.py voor meer informatie over de App klasse die jullie moeten implementeren.
"""

import argparse
import pathlib
import sys

from quetzal.app import App


if __name__ == "__main__":
    # laat de gebruiker toe om een inputbestand mee te geven als argument
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=pathlib.Path)
    args = parser.parse_args()

    # check of het inputbestand bestaat en exit met een foutmelding indien niet
    if not args.input_file.exists() or not args.input_file.is_file():
        print(f"Inputbestand bestaat niet: {args.input_file}", file=sys.stderr)
        exit(1)

    # lees het inputbestand
    with args.input_file.open(mode="r") as file:
        script = file.read()

        # voer het script uit
        app = App()
        resultaten = app.run(script)

        # maak een output directory aan met dezelfde naam als het inputbestand
        output_directory = args.input_file.with_suffix("")
        output_directory.mkdir(parents=True, exist_ok=True)

        # schrijf de log bestanden weg naar de output directory
        for tijdstip, log in resultaten.items():
            with open(output_directory / f"log{tijdstip}.html", "w") as file:
                file.write(log)
