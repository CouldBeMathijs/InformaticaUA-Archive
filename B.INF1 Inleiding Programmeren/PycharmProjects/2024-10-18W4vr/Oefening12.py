def add_matrices(matrix1, matrix2):
    if len(matrix1) != len(matrix2):
        return None
    for rij in range(len(matrix1)):
        if len(matrix1[rij]) != len(matrix2[rij]):
            return None
    resultaat = []
    for rij in range(len(matrix1)):
        rij_sommen = []
        for kolom in range(len(matrix1[rij])):
            rij_sommen.append(matrix1[rij][kolom] + matrix2[rij][kolom])
        resultaat.append(rij_sommen)
    return resultaat
