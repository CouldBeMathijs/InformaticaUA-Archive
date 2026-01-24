from decimal import Decimal, getcontext

# Set a higher precision to reduce IEEE 754 errors in our calculations
getcontext().prec = 50


def recursive_transform(number, times, mode="full"):
    if times <= 0:
        return  # Base case: stop recursion when times reaches 0

    # Step 1: Multiply the number by 2, using Decimal for precision
    number *= Decimal(2)

    # Step 2: Separate the integer and fractional parts
    integer_part = int(number)  # This will be 0 or 1
    fractional_part = number - integer_part  # The remaining fractional part

    # Step 3: Display based on the selected mode
    if mode == "full":
        # Print both integer and fractional parts
        print(f"{integer_part}.{str(fractional_part)[2:]}")
    elif mode == "fractional":
        # Print fractional part only if integer part is 1, else print as is
        print(fractional_part)
    elif mode == "binary":
        # Print only the integer part (0 or 1)
        print(integer_part)

    # Step 4: Recursive call with decremented times
    recursive_transform(fractional_part, times - 1, mode)


# Example usage:
initial_number = Decimal(input("Enter a number: "))
repetitions = int(input("Enter the number of times to repeat the process: "))
mode = input("Choose output mode ('full', 'fractional', or 'binary'): ").strip().lower()

recursive_transform(initial_number, repetitions, mode)