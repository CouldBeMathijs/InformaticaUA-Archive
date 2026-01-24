punt = int(input("Geef een punt op tussen 0 en 100 (incl.)"))
if punt>=90:
    print("grootste onderscheiding en de gelukwensen van de examencommissie")
elif 85<=punt<90:
    print("grootste onderscheiding")
elif 77<=punt<85:
    print("grote onderscheiding")
elif 68<=punt<77:
    print("onderscheiding")
elif 50<=punt<68:
    print("op voldoende wijze")
else:
    print("onvoldoende")