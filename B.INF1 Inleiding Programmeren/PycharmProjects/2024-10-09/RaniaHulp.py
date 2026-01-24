grootte = 9  #om het algemener te maken dan de 9 die gevraagd is,
# je kan ook deze variabele steeds vervangen door 9
lijn = ""
for rij in range(grootte): #buitenste lus, voor de rijen omdat je lijn per lijn print
    for kolom in range(grootte): #binnenste lus voor de waarde per kolom
        if (rij + kolom) % 2 == 0: #check of we op een even of oneven plaats staan
            lijn+="#" #concateneer "#" aan het einde van de variabele lijn
        else:
            lijn+=" " #concateneer " " aan het einde van de variabele lijn
    print(lijn)  # print de lijn variabele naar het scherm
    lijn = "" #maak de variabele lijn leeg, zodat deze opnieuw gebruikt kan worden
