def faculteit(n):
    nfaculteit = 1
    for i in range(1,n+1):
        nfaculteit*=i
    return nfaculteit
#print(faculteit(10))