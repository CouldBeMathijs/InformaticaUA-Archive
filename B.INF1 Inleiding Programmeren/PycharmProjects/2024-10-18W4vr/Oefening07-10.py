# Oefening 7
def create_dice():
    dice = []
    for i in range(13):
        dice.append(set())
    for d1 in range(1, 7):
        for d2 in range(1, 7):
            sum_dice = d1 + d2
            dice[sum_dice].add(tuple((d1, d2)))
    return dice
# Oefening 8
dice = create_dice()
def lose():
    dice = create_dice()
    return dice[2] | dice[3] | dice[12]
def win():
    dice = create_dice()
    return dice[7] | dice[11]
# Oefening 9
from random import randint
def roll_dice():
    d1 = randint(1, 6)
    d2 = randint(1, 6)
    return tuple([d1, d2])
# Oefening 10
def game_ends(worp):
    if worp in lose() or worp in win():
    	return True
    else:
        return False