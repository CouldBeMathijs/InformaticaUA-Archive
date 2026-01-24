def piramide(n):
    for i in range(1, n+1):
        aantal_cijfers = 2 * i - 1
        cijfers = ''
        for x in range(1, aantal_cijfers + 1):
            cijfers += str(x) + " "
        cijfers = cijfers.strip()
        spaties = "  " * (n - i)
        print(spaties + cijfers)
for i in range(8):
    piramide(i)
