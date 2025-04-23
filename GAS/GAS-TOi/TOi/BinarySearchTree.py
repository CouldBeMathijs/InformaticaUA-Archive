class Node:
    def __init__(self, value=None):
        """
        Geen deel van de BST, wel de elementen waaruit die zijn opgebouwd.
        Maakt een nieuwe node aan die nergens naar verwijst en wiens value meegegeven wordt als parameter.
        """
        self.value = value
        self.LeftChild = None
        self.RightChild = None

def createTreeItem(key,val):
    return Node(val)

class BST:
    def __init__(self):
        """
        Maakt een nieuwe binaire zoekboom
        precondition: de binaire zoekboom bestaat niet
        postcondition: de BST bestaat en is leeg
        """
        self.root = None
    def isEmpty(self):
        """
        Checkt of de zoekboom leeg is
        :return: (isEmpty:boolean)
        """
        return self.root is None
    def searchTreeInsert(self, newItem):
        """
        Voegt een nieuw item toe aan de zoekboom
        :param newItem: (newItem:ItemType)
        :return: (success:boolean)
        """
        # Als de boom leeg is, voeg het item dan toe als wortel
        if self.isEmpty():
            self.root = newItem
            return True
        else:
            # Anders, voeg het toe op de juiste plaats via de recursieve functie vanaf de wortel
            return self._insertRec(self.root, newItem)

    def _insertRec(self, current, newItem):
        # Als het nieuwe item kleiner is dan het huidige item
        if newItem.value < current.value:
            if current.LeftChild is None:
                # En de linkerdeelboom is leeg, voeg het hier dan toe
                current.LeftChild = newItem
                return True
            else:
                # Anders, voeg het toe op de juiste plaats via de recursieve functie vanaf de huidige plaats
                return self._insertRec(current.LeftChild, newItem)
        # Als het nieuwe item groter is dan het huidige item
        elif newItem.value > current.value:
            # En de rechterdeelboom is leeg, voeg het hier dan toe
            if current.RightChild is None:
                current.RightChild = newItem
                return True
            else:
                # Anders, voeg het toe op de juiste plaats via de recursieve functie vanaf de huidige plaats
                return self._insertRec(current.RightChild, newItem)
        return False  # Het item zat al in de zoekboom, en kan dus niet opnieuw toegevoegd worden
    def searchTreeDelete(self, searchKey):
        """
        Verwijdert een item uit de zoekboom aan de hand van zijn zoeksleutel
        :param searchKey: (searchKey:KeyType)
        :return: (success:boolean)
        """
        # Als de boom leeg is
        if self.isEmpty():
            # Kan het item niet verwijderd worden
            return False
        else:
            # Als de root veranderd is, na het oproepen van de _deleteRec functie, sla die op.
            self.root, deletedbool = self._deleteRec(self.root, searchKey)
            # En geef terug of de delete succesvol voltooid is
            return deletedbool

    def _deleteRec(self, current, key):
        # Als de delete wordt opgeroepen op een lege subtree
        if current is None:
            # Is de root niet veranderd, en de delete mislukt
            return current, False
        # Als de sleutel kleiner is dan de huidige waarde, zoek dan in de linkerdeelboom.
        if key < current.value:
            # Roept zichzelf recursief op
            current.LeftChild, deleted = self._deleteRec(current.LeftChild, key)
            # En geeft de eventueel veranderde waarden terug
            return current, deleted
        # Als de sleutel groter is dan de huidige waarde, zoek dan in de rechterdeelboom.
        elif key > current.value:
            # Roept zichzelf recursief op
            current.RightChild, deleted = self._deleteRec(current.RightChild, key)
            # En geeft de eventueel veranderde waarden terug
            return current, deleted
        # Anders is de huidige node, de nood die verwijderd moet worden
        else:
            # Geval 1: de node heeft 1 kind, of geen kinderen
            # Als er geen linkerkind is
            if current.LeftChild is None:
                # Geef het rechterkind terug (kan ook None zijn)
                return current.RightChild, True
            elif current.RightChild is None:
                # Geef het linkerkind terug (kan ook None zijn)
                return current.LeftChild, True

            # Geval 2: de node heeft 2 kinderen
            # zoek de kleinste node in de rechtersubboom (=inorder successor)
            min_larger_node = self._minValueNode(current.RightChild)
            # Vervang de waarde van de huidige node door de waarde van de inorder successor
            current.value = min_larger_node.value
            # Verwijder de nu gedupliceerde inorder successor uit de rechter subtree
            current.RightChild, _ = self._deleteRec(current.RightChild, min_larger_node.value)
            # Geef de aangepaste node terug
            return current, True
    def _minValueNode(self, root):
        #Ga steeds meer naar links totdat er geen meer zijn
        current = root
        while current.LeftChild is not None:
            current = current.LeftChild
        # En geef die terug
        return current

    def inorderTraverse(self, visit):
        """
        Inorder traversal of the binary search tree en roep een functie op voor elke knoop in de boom.
        :param visit: (function)
        """
        # Roep de recursieve functie op vanaf de wortel
        self._inorderTraverseRec(self.root, visit)

    def _inorderTraverseRec(self, node, visit):
        # Als de node niet None is (m.a.w. als de knoop bestaat)
        if node is not None:
            # Bezoek eerst de hele linkerzoekboom
            self._inorderTraverseRec(node.LeftChild, visit)
            # Dan de root
            visit(node.value)
            # En dan de hele rechterzoekboom
            self._inorderTraverseRec(node.RightChild, visit)

    def searchTreeRetrieve(self, searchKey):
        """
        zoek een specifiek item in een zoekboom
        :return: (searchKey:KeyType)
        :return: (success:boolean)
        """
        # Roep de recursieve functie op vanaf de wortel
        value, found = self._retrieveRec(self.root, searchKey)
        return value, found
    def _retrieveRec(self, current, key):
        # Als de deelboom leeg is, stop
        if current is None:
            return None, False
        # Als we de juiste knoop gevonden hebben
        if key == current.value:
            # Geef die terug
            return current.value, True
        # Als de gezochte key kleiner is dan de huidige, doorzoek alleen de linkerdeelboom
        elif key < current.value:
            return self._retrieveRec(current.LeftChild, key)
        # Als de gezochte key groter is dan de huidige, doorzoek alleen de rechterdeelboom
        else:
            return self._retrieveRec(current.RightChild, key)
    def save(self):
        """
        Slaat een binaire zoekboom op als een dictionary
        :return: (fullSearchTree:dictionary)
        """
        # Roep de recursieve functie op vanaf de wortel
        return self._saveRec(self.root)
    def _saveRec(self, node):
        # Als de knoop leeg is, geef None terug
        if node is None:
            return None
        # Als beide kinderen niet bestaan, geef alleen de wortel terug
        if node.LeftChild is None and node.RightChild is None:
            return {'root': node.value}
        # Anders bestaan beide kinderen, roep deze functie dan op, en geef deze weer als een dictionary
        return {
            'root': node.value,
            'children': [self._saveRec(node.LeftChild), self._saveRec(node.RightChild)]
        }
    def load(self, dictionary):
        """
        Laadt een dictionary als binaire zoekboom
        :param dictionary: (fullSearchTree:dictionary)
        """
        # Als de meegegeven dictionary bestaat
        if dictionary is not None:
            # Start vanaf de wortel, en vul zo de hele BST recursief in
            self.root = self._loadRec(dictionary)

    def _loadRec(self, dictionary):
        # Als de meegegeven dictionary niet bestaat, stop
        if dictionary is None:
            return None
        # Maak een nieuwe knoop aan uit de huidige dictionary key
        node = Node(dictionary['root'])
        # Maak de kinderen aan
        left_child = None
        right_child = None
        # Kijk na of 'children' bestaat, en minstens 2 elementen heeft
        if 'children' in dictionary:
            children = dictionary['children']
            # Als er minstens 1 kind is, is dit het linkerkind, en bouw deze subtree recursief verder op
            if len(children) > 0:
                left_child = self._loadRec(children[0])
            # Als er 2 kinderen zijn, is [1] het rechterkind, en bouw dan deze rechtersubtree recursief verder op
            if len(children) > 1:
                right_child = self._loadRec(children[1])
        # Geef de kinderen aan de knoop
        node.LeftChild = left_child
        node.RightChild = right_child
        # En geef de node terug
        return node


#testcode
if __name__ == "__main__":
    t = BST()
    print(t.isEmpty())
    print(t.searchTreeInsert(createTreeItem(8, 8)))
    print(t.searchTreeInsert(createTreeItem(5, 5)))
    print(t.isEmpty())
    print(t.searchTreeRetrieve(5)[0])
    print(t.searchTreeRetrieve(5)[1])
    t.inorderTraverse(print)
    print(t.save())
    t.load({'root': 10, 'children': [{'root': 5}, None]})
    t.searchTreeInsert(createTreeItem(15, 15))
    print(t.searchTreeDelete(0))
    print(t.save())
    print(t.searchTreeDelete(10))
    print(t.save())
# Expected output:
# True
# True
# True
# False
# 5
# True
# 5
# 8
# {'root': 8,'children':[{'root': 5},None]}
# False
# {'root': 10,'children':[{'root': 5},{'root': 15}]}
# True
# {'root': 15,'children':[{'root': 5},None]}