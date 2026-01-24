def som_veelvouden(a,b,c):
    som = 0
    for i in range(1,c):
        if i % a == 0 or i % b == 0:
            som+=i
    return som
print(som_veelvouden(2,3,4))
