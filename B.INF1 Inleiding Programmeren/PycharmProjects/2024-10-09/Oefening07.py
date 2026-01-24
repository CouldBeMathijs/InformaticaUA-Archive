def gemiddelde_lijst(lijst):
    totaal = 0
    if len(lijst) != 0:
        for i in range(len(lijst)):
            totaal += lijst[i]
        return totaal/len(lijst)

print(gemiddelde_lijst([]))