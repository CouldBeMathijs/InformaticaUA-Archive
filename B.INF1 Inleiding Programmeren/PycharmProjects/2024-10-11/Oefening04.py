from Oefening03 import faculteit
def variatie(n,k):
    return int(faculteit(n)/faculteit(n-k)) 

#print(variatie(3,2))