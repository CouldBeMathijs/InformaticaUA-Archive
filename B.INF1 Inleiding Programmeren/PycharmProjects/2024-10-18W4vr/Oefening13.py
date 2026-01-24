def multiply_matrices(m1, m2):
    # Check of vermenigvuldigen kan
    if len(m2) != len(m1[0]):
        return
    # Maak een nieuwe matrix aan, van de juiste vorm
    m3 = [[0 for _ in range(len(m2[0]))] for _ in range(len(m1))]
    # Vermenigvuldiging
    for rijm1 in range(len(m1)):
        for kolomm2 in range(len(m2[0])):
            for rijm2 in range(len(m2)):
                m3[rijm1][kolomm2] += m1[rijm1][rijm2] * m2[rijm2][kolomm2]
    return m3
