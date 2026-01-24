def is_palindrome(woord):
    for i in range(len(woord)//2):
        if woord[i]!=woord[-i-1]:
            return False
        else:
            return True

print(is_palindrome(input("Palindrome: ")))