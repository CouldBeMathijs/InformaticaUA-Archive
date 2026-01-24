g = int(input("Wat is getal 1? ")) #doen alsof het eerste de grootste is
k = int(input("Wat is getal 2? ")) #doen alsof het tweede de kleinste is
temp = 0
while g !=0:
    while g >= k:
            g = g - k
    if g != 0:
        temp = g
        g = k
        k = temp
print("De grootste gemene deler is", k)