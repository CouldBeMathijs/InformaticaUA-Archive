class MyQueue:
    def __init__(self, max_size):
        """
        constructor
        • krijgt max_size binnen die bepaald hoe groot de queue maximaal mag worden,
        • maakt een lege queue aan met maximale grote max_size,
        • geeft niets terug,
        • pre: de queue bestaat niet
        • post: de queue bestaat en is leeg
        """


        self.huidigequeue = [None] * max_size
        self.index = -1
        self.max_size = max_size

    def isEmpty(self):
        """
        isEmpty
        • geen input parameters
        • controleert of queue leeg is of niet en geeft dit terug in de bool
        • geeft een bool met succes,
        • pre: er bestaat een queue
        • post: /
        """


        if self.index == -1:
            return True
        else:
            return False

    def enqueue(self,newItem):
        """
        enqueue
        • een input parameter van het item dat bij de queue moet worden gezet
        • zet het parameter achteraan de queue
        • geeft een bool met succes,
        • pre: de queue is evengroot en er word een input waarde doorgegevn
        • post: er is een nieuw item  achteraan de queue
        """



        try:
            if self.index+1 < len(self.huidigequeue):
                self.index += 1
                self.huidigequeue[self.index] = newItem
                return True
            else:
                return False
        except:
            return False


    def getFront(self):
        """
        getFront
        • geen input parameters
        • kijk naar het voorste item van de queue en geeft dit mee
        • geeft een bool met succes en geeft het voorste van het queue mee,
        • pre: er bestaat een queue
        • post: de queue veranderd niet
        """


        output = (None, None)
        try:
            if(self.isEmpty()):
                output = (None, False)
                return output
            else:
                uitgave = self.huidigequeue[0]
                output = (uitgave, True)
                return output
        except:
            output = (None, False)
            return output

    def dequeue(self):
        """
        dequeue
        • geen input parameters
        • haalt het voorste item van de queue en geeft dit mee met ook een parameter van succes
        • geeft een bool met succes en het uitgehaalde item,
        • pre: de queue bestaat en is evengroot
        • post: de queue is 1 item kleiner, het voorste item is weg
        """


        output = (None,None)
        try:
            if (not self.isEmpty()):
                output = self.huidigequeue[0]
                self.huidigequeue[0] = None
                for i in range(self.index):
                    self.huidigequeue[i] = self.huidigequeue[i+1]
                self.huidigequeue[self.index] = None
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
        • zet de queue om in een lijst en geeft dit terug
        • geeft de queue in een lijst terug
        • pre: er bestaat een queue
        • post: /
        """


        return [item for item in self.huidigequeue if item is not None][::-1]

    def load(self,content):
        """
        load
        • een input parameter met een lijst waarmee we een queue aan gelijkstellen,
        • zet een lijst in een queue om meerdere items tegelijk in te voegen,
        • geen niets als parameter terug,
        • pre: er bestaat en queue
        • post: de queue word overgeschreven met de nieuwe lijst
        """

        content.reverse()
        self.huidigequeue = content
        self.index = len(content) -1
