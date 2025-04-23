def createTreeItem(key, value):
    """
    Maakt een tuple aan met als eerste waarde de sleutel die in de BST komt en de waarde achter de key.
    preconditie: Er moet een key en een value meegegeven worden.
    postconditie: Er is een tuple aangemaakt.
    :return: Een tuple van de sleutel en de waarde.
    """
    return (key, value)

class Node:
    def __init__(self, key, value):
        """
        Crëert een knoop in de boom met een verwijzing naar zijn eventuele linkerkind en eventuele rechterkind en de sleutel van de knoop en de waarde van de knoop.
        preconditie: /
        postconditie: Er is een knoop aangemaakt.
        :return: Er wordt niets teruggegeven.
        """
        self.left = None
        self.right = None
        self.key = key
        self.value = value

class BST:
    """
    Zet de root van de binaire zoekboom in de root.
    preconditie: /
    postconditie: De root van de boom staat hier in opgeslagen.
    :return: Er wordt niets teruggegeven.
    """
    def __init__(self):
        self.root = None

    def isEmpty(self):
        """
        Controleert of de binaire zoekboom leeg is.
        preconditie: /
        postconditie: Er wordt niets aangepast aan de binaire zoekboom.
        :return: True als de binaire zoekboom leeg is, anders False.
        """
        return self.root == None

    def searchTreeInsert(self, item):
        """
        Voeg een nieuw item toe aan de binaire zoekboom.
        preconditie: Er moet een tuple zijn aangemaakt (in createTreeItem) met 2 elementen (de key en de waarde).
        postconditie: De binaire zoekboom bevat het item in de correcte positie van de key, de rest van de binaire zoekboom is onaangepast.
        :return: True als het item succesvol is toegevoegd aan de binaire zoekboom, anders false.
        """
        if self.isEmpty():
            self.root = Node(item[0], item[1])
            return True
        return self.insert_rec(self.root, item)

    def insert_rec(self, current, item):
        if item[0] == current.key:
            return False
        elif item[0] < current.key:
            if current.left is None:
                current.left = Node(item[0],item[1])
                return True
            else:
                return self.insert_rec(current.left, item)
        else:
            if current.right is None:
                current.right = Node(item[0],item[1])
                return True
            else:
                return self.insert_rec(current.right, item)

    def searchTreeRetrieve(self, key):
        """
        Zoekt een item in de binaire zoekboom met dezelfde waarde als de meegegeven sleutel.
        preconditie: Er moet een geldige sleutel meegegeven worden.
        postconditie: Als de sleutel in de binaire zoekboom zit geeft het de value mee terug. En de binaire zoekboom blijft onverandert.
        :return: een tuple van (de waarde van de key, True) als de key gevonden is in de binaire zoekboom, ander geeft het (None, false) terug
        """
        node = self.retrieve_rec(self.root, key)
        if node is None:
            return None, False
        return node.value, True

    def retrieve_rec(self, current, key):
        if current is None:
            return None
        if key == current.key:
            return current
        elif key < current.key:
            return self.retrieve_rec(current.left, key)
        else:
            return self.retrieve_rec(current.right, key)

    def inorderTraverseValue(self, visit):
        """
        Bezoekt elke knoop's value in een in order volgorde in de binaire zoekboom.
        preconditie: Visit moet een heroproepbaare functie zijn.
        postconditie: Elke knoop in de binaire zoekboom wordt exact één keer bezocht in een in order volgorde.
        :return: Niets.
        """
        def inorder_rec(node):
            if node is not None:
                inorder_rec(node.left)
                visit(node.value)
                inorder_rec(node.right)
        inorder_rec(self.root)

    def inorderTraverseKey(self, visit):
        """
        Bezoekt elke knoop's key in een in order volgorde in de binaire zoekboom.
        preconditie: Visit moet een heroproepbaare functie zijn.
        postconditie: Elke knoop in de binaire zoekboom wordt exact één keer bezocht in een in order volgorde.
        :return: Niets.
        """
        def inorder_rec(node):
            if node is not None:
                inorder_rec(node.left)
                visit(node.key)
                inorder_rec(node.right)
        inorder_rec(self.root)

    def save(self):
        """
        Geeft de dictionary formaat terug van de meegegeven binaire zoekboom.
        preconditie: Er bestaat een binaire zoekboom.
        postconditie: Er wordt niets aangepast aan de binaire zoekboom.
        :return: een dictionary weergave van de binaire zoekboom.
        """
        def save_rec(node):
            if node is None:
                return None
            if [save_rec(node.left), save_rec(node.right)] != [None,None]:
                return {
                    'root': node.key,
                    'children': [save_rec(node.left), save_rec(node.right)]
                }
            else:
                return {
                    'root': node.key,
                }
        return save_rec(self.root)

    def load(self, data):
        """
        Laad een binaire zoekboom uit een meegegeven dictionary representatie.
        preconditie: De meegegeven data moet een correcte representatie zijn van de binaire zoekboom.
        postconditie: De data wordt op dezelfde manier in de binaire zoekboom gezet.
        :return: Niets.
        """
        def load_rec(data):
            if data is None:
                return None
            node = Node(data['root'], data['root'])
            if 'children' in data:
                node.left = load_rec(data['children'][0])
                node.right = load_rec(data['children'][1])
            return node
        self.root = load_rec(data)

    def searchTreeDelete(self, key):
        """
        Verwijdert een item uit de binaire zoekboom met dezelfde meegegeven sleutel.
        preconditie: Er moet een geldige key meegegevn worden.
        postconditie: Het item met dezelfde sleutel (als die bestaat) wordt correct uit de binaire zoekboom verwijdert, en de correcte inorder succesor wordt op de plaats gezet.
        :return: De verwijderde knoop en true als het item correct is verwijdert, anders geeft het false terug.
        """
        def delete_rec(node, key):
            if node is None:
                return node, False
            if key < node.key:
                node.left, deleted = delete_rec(node.left, key)
            elif key > node.key:
                node.right, deleted = delete_rec(node.right, key)
            else:
                deleted = True
                if node.left is None:
                    return node.right, deleted
                elif node.right is None:
                    return node.left, deleted
                successor = inorder_suc(node.right)
                node.key, node.value = successor.key, successor.value
                node.right, _ = delete_rec(node.right, successor.key)
            return node, deleted

        def inorder_suc(node):
            while node.left is not None:
                node = node.left
            return node

        self.root, deleted = delete_rec(self.root, key)
        return deleted

    def getLength(self):
        """
        Geeft de lengte van de BST terug
        preconditions: Er moet een correcte boom meegegeven
        postconditions: De correcte lengte van de boom wordt terug gegeven.
        :return: de lengte van de boom.
        """

        def count_nodes(node):
            if node is None:
                return 0
            return 1 + count_nodes(node.left) + count_nodes(node.right)

        return count_nodes(self.root)

class BSTTable(BST):
    """
    Wrapper voor mijn BST als een Table implementatie.
    """
    def tableIsEmpty(self):
        return self.isEmpty()
    def tableLength(self):
        return self.getLength()
    def tableInsert(self, newItem):
        return self.searchTreeInsert(createTreeItem(newItem[0],newItem[1]))
    def tableRetrieve(self,key):
        return self.searchTreeRetrieve(key)
    def traverseTableKeys(self,visit):
        self.inorderTraverseKey(visit)
    def traverseTableValues(self,visit):
        self.inorderTraverseValue(visit)
    def tableDelete(self, key):
        return self.searchTreeDelete(key)
