jaar = int(input("Geef een jaar op"))
if jaar%4==0:
    if jaar%100==0:
        if jaar%400==0:
            print("--schrikkeljaar--")
        else:
            print("--geen schrikkeljaar--")
    else:
        print("--schrikkeljaar--")
else:
    print("--geen schrikkeljaar--")