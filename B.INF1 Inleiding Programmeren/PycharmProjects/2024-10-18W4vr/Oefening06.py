def create_dice():
    dice = []
    for _ in range(13):
        dice.append(set())
    return dice
dice = create_dice()
print(dice)