n=1
nrij=1
epsilon=0.0000001
while True: # if x is approximately equal to y
    nrij = n/(n+1)
    if abs(nrij - 1) < epsilon:
        break
    n=n+1
print(n)