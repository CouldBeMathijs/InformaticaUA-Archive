dimx = int(input('lengte van de x-as: '))
dimy = int(input('lengte van de y-as: '))
b = int(input('b: '))
print("^")
Yas = "|"
Bas = "|"
Xas = "+"
for j in range(dimx):
    Xas = Xas + "-"
    Bas = Bas + "-"
for y in range(dimy):
    if y == dimy-b:
        print(Bas)
    else:
        print(Yas)
Xas = Xas + ">"
print(Xas)