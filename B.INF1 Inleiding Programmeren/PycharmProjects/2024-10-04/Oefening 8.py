basis = int(input('basis: '))
hoogte = int(input('hoogte: '))
lijn = ""
for i in range(basis):
    lijn = lijn + "*"
for i in range(hoogte):
    print(lijn)