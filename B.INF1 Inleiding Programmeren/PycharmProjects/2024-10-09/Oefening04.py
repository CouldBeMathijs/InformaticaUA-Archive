from Oefening03 import is_priem
def volgend_priemgetal(x):
    while True:
        x+=1
        if(is_priem(x)):
            return x

#test through user input
print(volgend_priemgetal(int(input("Wat is het priemgetal na input? "))))