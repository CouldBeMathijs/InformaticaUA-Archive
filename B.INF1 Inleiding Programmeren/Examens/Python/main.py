import random


class Game:

    def __init__(self, size=4):
        self.grid = []
        for _ in range(size):
            row = []
            for _ in range(size):
                row.append(0)
            self.grid.append(row)

    def printBoard(self):
        """ Print the board. PAS DEZE NIET AAN! """
        for row in self.grid:
            print(row)
        print("-" * 20)

    def getRow(self, index):
        """ Get the row at the given index """
        pass

    def getColumn(self, index):
        """ Get the column at the given index """
        pass

    def setRow(self, row, index):
        """ Set the row at the given index """
        pass

    def setColumn(self, column, index):
        """ Set the column at the given index """
        pass


    def spawn(self):
        """ Randomly spawn the number 2 in the grid where the value is 0 """
        x = random.randint(1, 10) # Picks an integer between 1 and 10
                                        # x can be 1, 2, ..., 9, 10

    def reverse(self, l):
        """ Reverse the given list """
        pass

    def move1(self, l):
        """ Move all the numbers in the given list to the left such that all zeroes are on the right side. """
        pass

    def move2(self, l):
        """ Combine all adjacent numbers that are the same. """
        pass

    def move(self, l, direction):
        """ Move the grid in the given direction """
        pass

    def moveUp(self):
        """ Move the whole grid up. """
        pass

    def moveDown(self):
        """ Move the whole grid down. """
        pass

    def moveLeft(self):
        """ Move the whole grid to the left. """
        pass

    def moveRight(self):
        """ Move the whole grid to the rigth. """
        pass

    def isSolved(self):
        """ Check if the game is solved. """
        pass

    def isLost(self):
        """ Check if the game is lost. """
        pass

# TODO: Deel 1
game = Game(4)
game.setRow([4, 16, 64, 2], 0)
game.setRow([2, 16, 8, 0], 1)
game.setRow([4, 0, 0, 2], 2)
game.setRow([2, 0, 0, 0], 3)

for i in range(4):
    print(game.getRow(i))

game.setColumn([4, 2, 4, 2], 0)
game.setColumn([16, 16, 0, 0], 1)
game.setColumn([64, 8, 0, 0], 2)
game.setColumn([2, 0, 2, 0], 3)

for i in range(4):
    print(game.getColumn(i))

# TODO: Deel 2
# game = Game(4)
# for i in range(16):
#     game.spawn()
# game.printBoard()

# TODO: Deel 3 (1)
#game = Game(4)
#print(game.reverse([4, 2, 4, 2]))
#print(game.reverse([16, 16, 0, 0]))
#print(game.reverse([64, 8, 0, 0]))
#print(game.reverse([2, 0, 2, 0]))

# TODO: Deel 3 (2)
#game = Game(4)
#print(game.move1([4, 2, 2, 4]))
#print(game.move1([16, 0, 0, 16]))
#print(game.move1([0, 0, 64, 8]))
#print(game.move1([2, 0, 2, 0]))

# TODO: Deel 3 (3)
#game = Game(4)
#print(game.move2([4, 2, 4, 2]))
#print(game.move2([16, 16, 0, 0]))
#print(game.move2([64, 8, 0, 8]))
#print(game.move2([2, 2, 2, 2]))
#print(game.move2([0, 0, 2, 2]))

# TODO: Deel 4
#game = Game(4)
#for d in [False, True]:
#    print(game.move([4, 2, 4, 2], d))
#    print(game.move([16, 16, 0, 0], d))
#    print(game.move([64, 8, 0, 0], d))
#    print(game.move([2, 0, 2, 0], d))
#    print(game.move([2, 2, 2, 2], d))
#    print(game.move([0, 0, 2, 2], d))

# TODO: Deel 5
#game = Game(4)
#game.setRow([4, 16, 64, 8], 0)
#game.setRow([2, 16, 8, 0], 1)
#game.setRow([2, 0, 0, 8], 2)
#game.setRow([4, 0, 0, 0], 3)
#game.moveUp()
#game.printBoard()
#game.moveRight()
#game.printBoard()
#game.moveDown()
#game.printBoard()
#game.moveLeft()
#game.printBoard()
#game.moveRight()
#game.printBoard()
#game.moveDown()
#game.moveDown()
#game.printBoard()
#game.moveLeft()
#game.moveRight()
#game.moveUp()
#game.printBoard()

# TODO: Deel 6
#game = Game(4)
#print(game.isSolved())
#print(game.isLost())
#game.setRow([2048, 0, 0, 0], 0)
#print(game.isSolved())
#print(game.isLost())
#game.setRow([2, 4, 8, 16], 0)
#game.setRow([16, 8, 4, 2], 1)
#game.setRow([2, 4, 8, 16], 2)
#game.setRow([16, 8, 4, 2], 3)
#print(game.isSolved())
#print(game.isLost())
#game.setRow([16, 8, 4, 16], 1)
#print(game.isLost())



