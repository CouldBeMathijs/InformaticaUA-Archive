def splits(lijst):
    links = lijst[:len(lijst)//2]
    if len(lijst)%2 == 0:
        rechts = lijst[(len(lijst)//2):]
    else:
        rechts = lijst[(len(lijst)//2)+1:]
    return tuple((links, rechts))
print(splits(['a', 'b', 'c', 'd', 'e', 'f']))
