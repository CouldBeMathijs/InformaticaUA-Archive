##########
## Gebruik volgende functies om te bewegen in de doolhof:
## 
## goForward():
##   Beweeg 1 vakje vooruit
##
## turnRight():
##   Draai een kwartslag naar rechts
##
## turnLeft():
##   Draai een kwartslag naar links
##########

from doolhof import *

# Probleem 1: Manueel uit doolhof gaan
laadDoolhof("doolhof1.txt")
turnRight()
goForward()
turnLeft()

# laat deze regel staan zodat het venster blijft openstaan
turtle.done()