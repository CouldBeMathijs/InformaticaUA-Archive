def tel_getallen(l):
    l.sort()
    count = []
    for number in range(max(l)+1):
        count.append(str(number) + " " + str(l.count(number)))
    for lijn in count:
        print(lijn)

tel_getallen([1,2,2,6,7,88,9,88,3,4,2])
