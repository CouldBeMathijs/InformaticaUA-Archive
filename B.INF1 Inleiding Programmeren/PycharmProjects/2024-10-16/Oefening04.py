from alice_book import book
# woorden is een lijst van alle woorden uit het boek
woorden = book.split()
def tel_woorden(woorden):
    woord_aantal = {}
    for woord in woorden:
        if woord in woord_aantal:
            woord_aantal[woord] += 1
        else:
            woord_aantal[woord] = 1
    return woord_aantal
#print(tel_woorden(woorden))
