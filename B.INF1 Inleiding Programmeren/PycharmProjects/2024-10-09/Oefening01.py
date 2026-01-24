def integer_division(deeltal, deler):
    """
    doe de gehele deling van deeltal door deler
    """
    quotient = 0
    while deeltal >= deler:
        quotient += 1
        deeltal -= deler
    return quotient

print(integer_division(7, 3))
print(integer_division(123, 5))
print(integer_division(0, 7))
print(integer_division(7, 7))
print(integer_division(15, 7))