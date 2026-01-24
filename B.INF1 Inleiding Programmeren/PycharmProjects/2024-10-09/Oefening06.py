import math
def priemfactoren(x):
    pf = []
    getal = 2
    while x > 1:
        while x % getal == 0:
            pf.append(getal)
            x = x / getal
        getal += 1
    return pf
print(priemfactoren(1024))

