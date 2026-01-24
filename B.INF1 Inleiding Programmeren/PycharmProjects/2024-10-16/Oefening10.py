from Oefening09 import fibonacci
from Oefening08 import priemgetallen
def priem_en_fib(n):
    return list(set(fibonacci(n)) & set(priemgetallen(n)))

print(priem_en_fib(10))


