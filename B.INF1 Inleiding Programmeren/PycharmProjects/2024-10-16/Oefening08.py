#Availible functions
import math
def is_priem(x):
    # Priemgetallen zijn groter dan 1
    if x < 2:
        return False
    # Controleer deelbaarheid door 2 tot en met de wortel van x
    for i in range(2, int(math.sqrt(x)) + 1):
        if x % i == 0:
            return False
    return True
def volgend_priemgetal(x):
    while True:
        x+=1
        if(is_priem(x)):
            return x
#Eigenlijke functie
def priemgetallen(n):
    uitkomst = []
    priem = 2
    while len(uitkomst) < n:
        uitkomst.append(priem)
        priem = volgend_priemgetal(priem)
    return uitkomst
#Test
#print(priemgetallen(13))
