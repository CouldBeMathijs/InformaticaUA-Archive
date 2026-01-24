def midden(A, B):
    if len(A)!=len(B):
        return
    uitkomst = []
    for dimension in range(len(A)):
        uitkomst.append((A[dimension]+B[dimension])/2)
    return tuple(uitkomst)

print(midden((2,2,2),(2,4,0)))


