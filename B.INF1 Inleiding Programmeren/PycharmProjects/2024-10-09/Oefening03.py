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
#test door user input