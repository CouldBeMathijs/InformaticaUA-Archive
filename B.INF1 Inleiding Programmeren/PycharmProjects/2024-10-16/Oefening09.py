def fibonacci(n):
    if n == 0:
        fib = []
    elif n == 1:
        fib = [1]
    else:
        fib = [1, 1]
    for i in range(n - 2):
        fib.append(fib[-1] + fib[-2])
    return fib

#print(fibonacci(6))