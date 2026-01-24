import math


def is_prime(n):
    """
    Check whether n is a prime number.

    :param n: the number being tested
    :return: True if the number is prime, False otherwise

    >>> is_prime(2)
    True
    >>> is_prime(4)
    False
    """
    if n <= 1:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False

    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False

    return True