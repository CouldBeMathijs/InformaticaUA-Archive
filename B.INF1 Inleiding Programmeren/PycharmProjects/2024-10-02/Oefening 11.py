times = int(input("Hoeveel keer moet men 'We like Python tonen'?"))
reallys = int(input("Hoeveel keer moet er really staan per lijn?"))
if reallys!=0:
    reallyString = "really, " * (reallys - 1) + "really"
else:
    reallyString = ""
for i in range(0,times):
    print("We",reallyString,"like Python!")