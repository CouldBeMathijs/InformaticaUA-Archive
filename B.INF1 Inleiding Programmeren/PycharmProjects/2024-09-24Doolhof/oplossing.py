from doolhof import *

# Problem 1: 
laadDoolhof("doolhof1.txt")
turnRight()
goForward()
turnLeft()
goForward()
goForward()
turnRight()
goForward()
goForward()
turnLeft()
goForward()
goForward()
turnLeft()
goForward()
goForward()
goForward()


# Problem 2a: use while
laadDoolhof("doolhof2a.txt")

while freeForward():
    goForward()
turnRight()
while freeForward():
    goForward()
turnRight()
while freeForward():
    goForward()

# 2a alternatif solution
laadDoolhof("doolhof2a.txt")

while not foundExit():
    while freeForward():
        goForward()
    turnRight()

# 2b: while assignment b
laadDoolhof("doolhof2b.txt")

while not foundExit():
    while freeForward():
        goForward()
    turnLeft()

# Problem 3: Solve doolhof3 using while and if - elif - else
laadDoolhof("doolhof3.txt")    

while not foundExit():
    if freeLeft():
        turnLeft()
        goForward()
    elif freeRight():
        turnRight()
        goForward()
    else:
        goForward()

# problem 4: Right-hand rule
laadDoolhof("doolhof4.txt")

while not foundExit():
    if freeRight(): # right is free
        turnRight()
        goForward()
    elif freeForward(): # not right, but forward is free
        goForward()
    elif freeLeft():  # not right, not forward, but left free
        turnLeft()    
        goForward
    else: # neither right, nor forward, nor left are free
        turnLeft()
        turnLeft()
        goForward()

# problem 4 alternatif solution
laadDoolhof("doolhof4.txt")

while not foundExit():
    if freeRight(): # right is free
        turnRight()
    elif not freeForward() and freeLeft():  # right and forward not free, but left is
        turnLeft()                          
    elif not freeForward(): # neither right, nor forward, nor left are free
        turnLeft()
        turnLeft()
    goForward()

# problem 4, with doolhof 5
laadDoolhof("doolhof5.txt")

while not foundExit():
    if freeRight():
        turnRight()
    while not freeForward():
        turnLeft()
    goForward()

# problem 5
# The following code counts the number of steps to solve the doolhof
# Question 5 is to add 2 variables to count the number of left
# (resp. right) turns

# Right-hand rule
laadDoolhof("doolhof4.txt")
steps=0
left=0
right=0
while not foundExit():
    steps=steps+1
    if freeRight(): # right is free
        turnRight()
        right=right+1
        goForward()
    elif freeForward(): # not right, but forward is free
        goForward()
    elif freeLeft():  # not right, not forward, but left free
        turnLeft()
        left=left+1
        goForward()
    else: # neither right, nor forward, nor left are free
        turnLeft()
        turnLeft()
        left=left+2
        goForward()

print("Number of steps:",steps)
print("Number of left turns:",left)
print("Number of right turns:",right)


# problem 6
# Extend the code such that also the number of steps of the
# left-hand rule is counted and it is output which one takes the
# least steps.

from doolhof import *

# Right-hand rule
laadDoolhof("doolhof4.txt")

stepsRHR=0
while not foundExit():
    stepsRHR=stepsRHR+1
    if freeRight(): # right is free
        turnRight()
        goForward()
    elif freeForward(): # not right, but forward is free
        goForward()
    elif freeLeft():  # not right, not forward, but left free
        turnLeft()
        goForward()
    else: # neither right, nor forward, nor left are free
        turnLeft()
        turnLeft()
        goForward()

# Right-hand rule
laadDoolhof("doolhof4.txt")
# Now do the Lefthand rule and count steps
stepsLHR=0
while not foundExit():
    stepsLHR=stepsLHR+1
    if freeLeft(): # left is free
        turnLeft()
        goForward()
    elif freeForward(): # not left, but forward is free
        goForward()
    elif freeRight():  # not left, not forward, but right free
        turnRight()
        goForward()
    else: # neither right, nor forward, nor left are free
        turnRight()
        turnRight()
        goForward()

# Decide which one is larger and print output
if stepsLHR==stepsRHR:
    print("Beiden nemen hetzelfde aantal stappen")
elif stepsLHR<stepsRHR:
    print("Linkerhand regel is efficienter:",stepsLHR,"vs.",stepsRHR)
else:
    print("Rechterhand regel is efficienter:",stepsRHR,"vs.",stepsLHR)

turtle.done()



### aMAZEing ! 


