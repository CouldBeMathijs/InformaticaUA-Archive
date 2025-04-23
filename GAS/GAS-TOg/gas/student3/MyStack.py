

class MyStack:
    def __init__(self, max_size):
        """
        constructor
        • krijgt max_size binnen die bepaald hoe groot de stack maximaal mag worden,
        • maakt een lege stack aan met maximale grote max_size,
        • geeft niets terug,
        • pre: de stack bestaat niet
        • post: de stack bestaat en is leeg
        """


        self.huidigestack = [None] * max_size
        self.index = -1

    def isEmpty(self):
        """
        isEmpty
        • geen input parameters
        • controleert of stack leeg is of niet en geeft dit terug in de bool
        • geeft een bool met succes,
        • pre: er bestaat een stack
        • post: /
        """


        if self.index == -1:
            return True
        else:
            return False

    def push(self,newItem):
        """
        push
        • een input parameter van het item dat op de stack moeten worden gepushed
        • zet het parameter item bovenop de stack
        • geeft een bool met succes,
        • pre: de stack is evengroot en er word een input waarde doorgegevn
        • post: er is een nieuw item  bovenop de stack
        """



        try:
            if self.index+1 < len(self.huidigestack):
                self.index += 1
                self.huidigestack[self.index] = newItem
                return True
            else:
                return False
        except:
            return False


    def getTop(self):
        """
        getTop
        • geen input parameters
        • kijk naar het bovenste item van de stack en geeft dit mee
        • geeft een bool met succes en geeft de bovenste van het stack mee,
        • pre: er bestaat een stack
        • post: de stack veranderd niet
        """


        output = (None, None)
        try:
            if(self.isEmpty()):
                output = (None, False)
                return output
            else:
                uitgave = self.huidigestack[self.index]
                output = (uitgave, True)
                return output
        except:
            output = (None, False)
            return output

    def pop(self):
        """
        pop
        • geen input parameters
        • haalt het bovenste item van de stack en geeft dit mee met ook een parameter van succes
        • geeft een bool met succes en het uitgehaalde item,
        • pre: de stack bestaat en is evengroot
        • post: de stack is 1 item kleiner, het bovenste item is weg
        """


        output = (None,None)
        try:
            if (not self.isEmpty()):
                output = self.huidigestack[self.index]
                self.huidigestack[self.index] = None
                self.index -= 1
                output = (output,True)
                return output
            else:
                output = (None, False)
                return output
        except:
            output = (None,False)
            return output

    def save(self):
        """
        save
        • geen input parameters
        • zet de stack om in een lijst en geeft dit terug
        • geeft de stack in een lijst terug
        • pre: er bestaat een stack
        • post: /
        """


        return [item for item in self.huidigestack if item is not None]

    def load(self,content):
        """
        load
        • een input parameter met een lijst waarmee we een stack aan gelijkstellen,
        • zet een lijst in een stack om meerdere items tegelijk in te voegen,
        • geen niets als parameter terug,
        • pre: er bestaat en stack
        • post: de stack word overgeschreven met de nieuwe lijst
        """


        self.huidigestack = content
        self.index = len(content) -1


