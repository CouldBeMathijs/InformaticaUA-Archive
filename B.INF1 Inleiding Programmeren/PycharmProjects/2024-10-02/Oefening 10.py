times = int(input("Hoeveel keer moet men 'We like Python tonen'?"))
reallys = int(input("Hoeveel keer moet er really staan per lijn?"))
reallyString = "really " * reallys
for i in range(0,times):
    print("We",reallyString+"like Python!")