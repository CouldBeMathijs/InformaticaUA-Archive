def remove_multiples(l,n):
    for counter in l[:]:
        if counter%n == 0:
            l.remove(counter)
l = [1,2,3,4,5,6,7,8,9,12,15]
print(remove_multiples(l,3))
print(l)