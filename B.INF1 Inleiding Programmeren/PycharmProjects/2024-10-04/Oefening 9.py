basis = int(input('basis: '))
hoogte = int(input('hoogte: '))
lijntop = "*"
lijnmid = "*"
for i in range(basis-2):
    lijntop = lijntop + "*"
    lijnmid = lijnmid + " "
lijntop = lijntop + "*"
lijnmid = lijnmid + "*"
print(lijntop)
for i in range(hoogte-2):
    print(lijnmid)
print(lijntop)
