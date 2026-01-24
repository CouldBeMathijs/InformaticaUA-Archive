dimx = int(input('lengte van de x-as: '))
dimy = int(input('lengte van de y-as: '))
print("^")
for i in range(dimy):
    print("|")
Xas = "+"
for j in range(dimx):
    Xas = Xas + "-"
Xas = Xas + ">"
print(Xas)