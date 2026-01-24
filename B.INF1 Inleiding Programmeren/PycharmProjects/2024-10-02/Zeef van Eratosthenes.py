limiet = int(input("Tot welk getal wil je controleren? ")) + 1
zeef = []
print("De priemgetallen tot",limiet-1,"zijn:")
for i in range(limiet):
    zeef.append(i)
zeef[0] = '/'
zeef[1] = '/'
for i in range(2,limiet):
    if zeef[i] != "/":
        print(zeef[i])
        for j in range(2*i, limiet, i):
            zeef[j] = "/"
