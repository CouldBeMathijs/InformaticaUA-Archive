l1 = [1, 2, 6, 8, 16, 67, 128, 890]
l2 = [3, 5, 12, 34]

l = []

i = 0; j = 0

for _ in range(len(l1) + len(l2)):
    if i >= len(l1):
        l.append(l2[j])
        j += 1
    elif j >= len(l2):
        l.append(l1[i])
        i += 1
    elif l1[i] < l2[j]:
        l.append(l1[i])
        i += 1
    else:
        l.append(l2[j])
        j += 1

print(l)
