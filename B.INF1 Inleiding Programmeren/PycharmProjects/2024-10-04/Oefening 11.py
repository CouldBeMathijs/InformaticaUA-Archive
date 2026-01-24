zijde = int(input('zijde: '))
print("*")
lijnlijst = []
nieuwelijn = "*"
for i in range(zijde-2):
    nieuwelijn = "*" + "."*i + "*"
    lijnlijst.append(nieuwelijn)
lijnlijst.append("*"*(zijde))
for i in lijnlijst:
    print(i)