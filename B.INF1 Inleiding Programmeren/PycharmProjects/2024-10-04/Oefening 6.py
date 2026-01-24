dimx = int(input('lengte van de x-as: '))
dimy = int(input('lengte van de y-as: '))
a = int(input('a: '))
print("^")
Yas = "|"
for i in range(a):
    Yas = Yas + " "
Yas = Yas + "|"
for y in range(dimy):
    print(Yas)
Xas = "+"
for j in range(dimx):
    Xas = Xas + "-"
Xas = Xas + ">"
print(Xas)