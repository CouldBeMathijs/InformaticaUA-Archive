def difference(l1, l2):
    difference = []
    for element in l1:
        if element not in l2:
            difference.append(element)
    return difference

print(difference([1, 2], [3, 4]))          # Expected: [1, 2]
print(difference([1, 2], [1, 3, 4]))       # Expected: [2]
print(difference([1, 2], [3, 4, 1]))       # Expected: [2]
print(difference([1, 2], [1, 3, 4, 2]))    # Expected: []
print(difference([1, 2, 3], [1, 5, 7, 2]))  # Expected: [3]