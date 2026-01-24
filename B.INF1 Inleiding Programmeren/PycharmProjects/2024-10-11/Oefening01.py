import math
def hms(seconds):
    hmslist = []
    seconds = int(seconds)
    hmslist.append(math.floor(seconds/3600))
    seconds = seconds % 3600
    hmslist.append(math.floor(seconds/60))
    seconds = seconds % 60
    hmslist.append(math.floor(seconds))
    return hmslist
print(hms(3600))
print(hms(23456))

