def minimum_elements(lijst):
    if not lijst:
        return
    kleinste = lijst[0]
    for element in lijst:
        if element < kleinste:
            kleinste = element
    teller =  0
    for element in lijst:
        if element == kleinste:
            teller+=1
    return (kleinste, teller)
