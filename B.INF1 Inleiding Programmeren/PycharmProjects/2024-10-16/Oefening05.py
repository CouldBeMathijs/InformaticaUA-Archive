from Oefening04 import tel_woorden
from alice_book import book
woorden = book.split()
#Eigenlijke functie

def print_frequenties(woord_aantal):
    for woord in sorted(woord_aantal):
        print(woord, woord_aantal[woord])
print_frequenties(woorden)