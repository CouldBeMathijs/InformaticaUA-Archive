l1 = [5, 1, 2]; l2 = [5, 2, 3, 1]
doorsnede = []
for i in l1:
    for j in l2:
        if i == j:
            doorsnede.append(i)
doorsnede.sort()
print(doorsnede)