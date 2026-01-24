def maak_sudoku():
    """
        Maak nieuwe sudoku aan met op alle posities None
    """
    sudoku = []
    for i in range(9):
        rij = [None, None, None, None, None, None, None, None, None]
        sudoku.append(rij)
    return sudoku


def dubbels(rij):
    """
        Geef True terug als een element twee keer voorkomt in 'rij'
    """
    checkrij = []
    for e in range(len(rij)):
        if rij[e] in checkrij and not rij[e] == None:
            return True
        checkrij.append(rij[e])
    return False


def geldig(sudoku):
    """
        Kijkt na of de gegeven sudoku geldig is
    """
    for i in range(9):
        kolom = []
        for j in range(9):
            kolom.append(sudoku[j][i])
        rij = sudoku[i]
        if (dubbels(rij) or dubbels(kolom)):
            return False

    # TODO in vraag 4.3: Code om de 9 blokken na te kijken op duplicaten
    for i in range(0, 9, 3):
        for j in range(0, 9, 3):
            blok = []
            for k in range(3):
                for l in range(3):
                    blok.append(sudoku[i+k][j+l])
            if dubbels(blok):
                return False
    return True

def print_sudoku(s):
    """
        Help functie om een sudoku mooi uit te printen
    """
    print("+-----------------+-----------------+-----------------+")
    for x in range(9):
        print("|", end="")
        for y in range(9):
            if s[x][y] is not None:
                print(" ", s[x][y], " ", end="")
            else:
                print("  _  ", end="")
            if ((y+1)%3) != 0:
                print(" ", end="")
            else:
                print("|", end="")
        print()
        if ((x+1)%3 == 0):
            print("+-----------------+-----------------+-----------------+")
        else:
            print("|                 |                 |                 |")


if __name__ == "__main__":
    # Begin debugging
    s1 = maak_sudoku()
    print_sudoku(s1)

    s1[0][0] = 5
    s1[1][2] = 3
    s1[7][7] = 2
    s1[8][8] = 4

    print(geldig(s1))
    s1[0][1] = 5
    print(geldig(s1))

    print("Ben je tot hier geraakt en zijn de vorige lijnen true en false?")
    # EINDE debugging

    # Begin uitbreiden
    s2 = [[1, None, None, None, None, None, None, None, 6],
      [None, 1, 6, None, 2, None, 7, None, None],
      [7, 8, 9, 4, 5, None, 1, None, 3],
      [None, None, None, 8, None, 7, None, None, 4],
      [None, None, None, None, 3, None, None, None, None],
      [None, 9, None, None, None, 4, 2, None, 1],
      [3, None, 2, 9, 7, None, None, 4, None],
      [None, 4, None, None, 1, 2, None, 7, 8],
      [9, None, 8, None, None, None, None, None, None]]
    print_sudoku(s2)
    print(geldig(s2))
    # geldig(s2) geeft foutief "true" het blokje links bovenaan(vierkantje opgespannen door[0][0] en[2][2]) bevat twee maal het cijfer 1
    # breid geldig uit zodat ook de vierkantjes nagekeken worden

    # EINDE code uitbreiden

    # Begin nieuwe functionaliteit
    s3 = [[1, None, None, None, None, None, None, None, 6],
      [None, None, 6, None, 2, None, 7, None, None],
      [7, 8, 9, 4, 5, None, 1, None, 3],
      [None, None, None, 8, None, 7, None, None, 4],
      [None, None, None, None, 3, None, None, None, None],
      [None, 9, None, None, None, 4, 2, None, 1],
      [3, 1, 2, 9, 7, None, None, 4, None],
      [None, 4, None, None, 1, 2, None, 7, 8],
      [9, None, 8, None, None, None, None, None, None]]

    # print(mogelijk(s3, 1, 1)) # Geeft: [3, 5]
    # print(mogelijk(s3, 4, 5)) # Geeft: [1, 5, 6, 9]
    # print(mogelijk(s3, 8, 8)) # Geeft: [2, 5]

    s4 = [[None, None, None, None, None, None, None, None, 6],
      [None, None, 6, None, 2, None, 7, None, None],
      [7, 8, 9, 4, 5, None, 1, None, 3],
      [None, None, None, 8, None, 7, None, None, 4],
      [None, None, None, None, 3, None, None, None, None],
      [None, 9, None, None, None, 4, 2, None, 1],
      [3, 1, 2, 9, 7, None, None, 4, None],
      [None, 4, None, None, 1, 2, None, 7, 8],
      [9, None, 8, None, None, None, None, None, None]]
    print_sudoku(s4)
    # vul_aan(s4)
    print_sudoku(s4)