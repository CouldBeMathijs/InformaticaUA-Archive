def reverse(woord):
    woord_lijst = []
    droow_lijst = []
    droow = ""
    for letter in range(len(woord)):
        woord_lijst.append(woord[letter])
    for rettel in woord_lijst:
        droow_lijst.insert(0, rettel)
    for element in droow_lijst:
        droow+=element
    return droow

print(reverse("Gezondheid"))