def som_veelvouden(n):
    som = 0
    for i in range(1,n):
        if i % 3 == 0 or i % 5 == 0:
            som+=i
    return som

print(som_veelvouden(10))