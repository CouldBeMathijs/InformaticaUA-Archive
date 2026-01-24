def createTreeItem(key, value):
    """
    Maakt een tuple aan met als eerste waarde de sleutel die in de BST komt en de waarde achter de key.
    preconditie: Er moet een key en een value meegegeven worden.
    postconditie: Er is een tuple aangemaakt.
    :return: Een tuple van de sleutel en de waarde.
    """
    return (key, value)

class Node:
    def __init__(self): ## data
        self.keys = []
        self.children = []
        self.parent = None

    def insert_key(self, key):
        """
        Voegt een key toe aan de lijst met keys. En sorteert de lijst terug.
        preconditie: De key moet een geldige waarde bevatten.
        postconditie: De key is toegevoegd aan de lijst en de lijst is gesorteerd.
        :return: None.
        """
        self.keys.append(key)
        self.keys = sorted(self.keys, key=lambda x: x[0])

    def is_leaf(self):
        """
        Controleert of de Node een blad is.
        preconditie: /
        postconditie: Er is niets verandert aan de boom.
        :return: True als de Node een blad is, anders false.
        """
        return len(self.children) == 0

class TwoThreeTree:
    def __init__(self):
        """Bevat de root van de boom."""
        self.root = None

    def isEmpty(self):
        """
        Controleert of de boom leeg is.
        preconditie: /
        postconditie: De boom wordt niet aangepast
        :return: True als de boom leeg is, anders false.
        """
        return self.root == None

    def retrieveItem(self, key):
        """
        Zoekt de key in de boom.
        preconditie: De key moet een geldige waarde bevatten.
        postconditie: De boom is niet aangepast.
        :return: De key als die in de boom zit gevolgd door True, anders None, False.
        """
        if self.isEmpty():
            return None, False
        current = self.root
        loop = False
        while loop is False:
            for i in current.keys:
                if key == i[0]:
                    loop = True
                    index_key = self.index_key(current, key)
                    return current.keys[index_key][1], True
            if current.is_leaf():
                return None, False
            if key < current.keys[0][0]:
                current = current.children[0]
            elif (len(current.keys) > 1 and key < current.keys[1][0]) or len(current.keys) == 1:
                current = current.children[1]
            else:
                current = current.children[2]
        return None, False

    def insertItem(self, item):
        """
        Voegt een item toe aan de boom.
        preconditie: Het item is een tuple met een key en een value. Alle zoeksleutels in tree zijn uniek en verschillen ook van de zoeksleutel van newItem.
        postconditie: Het item is op de correcte plaats in de boom toegevoegd.
        :return: True als het item succesvol is toegevoegd, anders False.
        """
        if self.isEmpty():
            self.root = Node()
            self.root.insert_key(item)
            return True
        leaf = self.retrieveItem_insert(item)
        if leaf is None:
            return False
        leaf.insert_key(item)
        if len(leaf.keys) > 2:
            self.split(leaf)
        return True

    def split(self, node):
        """
        Splitst de knoop n die 3 items bevat.
        preconditie: De meegegeven knoop moet 3 items bevatten. Als n geen blad is, dan heeft n 4 kinderen.
        postconditie: De Node is correct gesplitst.
        :return: Niets
        """
        middle_key = node.keys[1]
        left_node = Node()
        right_node = Node()
        left_node.keys = node.keys[:1]
        right_node.keys = node.keys[2:]

        if not node.is_leaf():
            left_node.children = node.children[:2]
            right_node.children = node.children[2:]
            for child in left_node.children:
                child.parent = left_node
            for child in right_node.children:
                child.parent = right_node

        if node.parent is None:
            # If the node is the root, create a new root
            new_root = Node()
            new_root.keys = [middle_key]
            new_root.children = [left_node, right_node]
            left_node.parent = new_root
            right_node.parent = new_root
            self.root = new_root

        else:
            # Insert the middle key into the parent
            parent = node.parent
            parent.insert_key(middle_key)

            index = parent.children.index(node)
            parent.children.pop(index)
            parent.children.insert(index, left_node)
            parent.children.insert(index+ 1, right_node)
            left_node.parent = parent
            right_node.parent = parent

            if len(parent.keys) > 2:
                self.split(parent)


    def retrieveItem_insert(self, key): # returns the tuple of th item
        """
        Zoekt het blad waarin het newItem volgens z’n zoeksleutel hoort.
        preconditie: Een geldige key.
        postconditie: Er is niets verandert aan de boom.
        :return: Het blad waar het item hoort volgens de key.
        """
        if self.isEmpty():
            return None
        current = self.root
        while not current.is_leaf():
            for i in current.keys:
                if key[0] == i[0]:
                    return None
            if key[0] < current.keys[0][0]:
                current = current.children[0]
            elif (len(current.keys) > 1 and key[0] < current.keys[1][0]) or len(current.keys) == 1:
                current = current.children[1]
            else:
                current = current.children[2]
        for i in current.keys:
            if key[0] == i[0]:
                return None
        return current

    def save(self, node=None):
        """
        Maakt een dictionary voorstelling van de boom.
        preconditie: Er moet een geldige boom bestaan.
        postconditie: Er is niets verandert aan de boom.
        :return: Een dictionary voorstelling van de 2-3 boom.
        """
        if node is None:
            node = self.root

        if self.root is None:
            return {"root": None}

        node_representation = dict()
        node_representation["root"] = [tup[0] if isinstance(tup, tuple)else tup for tup in node.keys]

        # If the node has children, recursively save them
        if not node.is_leaf():
            node_representation['children'] = []
            for child in node.children:
                node_representation['children'].append(self.save(child))

        return node_representation

    def inorderTraverseKey(self, visit):
        """
        Doorloopt de boom in inorder-volgorde en bezoekt de node's keys.
        preconditie: Er moet een geldige boom bestaan.
        postconditie: Er is niets veranderd aan de boom.
        :return: Niets
        """
        def traverse(node):
            if node.is_leaf():
                for key in node.keys:
                    visit(key[0])
            else:
                for i in range(len(node.keys)):
                    traverse(node.children[i])
                    visit(node.keys[i][0])
                traverse(node.children[-1])

        if not self.isEmpty():
            traverse(self.root)

    def inorderTraverseValue(self, visit):
        """
        Doorloopt de boom in inorder-volgorde en bezoekt de node's value.
        preconditie: Er moet een geldige boom bestaan.
        postconditie: Er is niets veranderd aan de boom.
        :return: Niets
        """

        def traverse(node):
            if node.is_leaf():
                for key in node.keys:
                    visit(key[1])
            else:
                for i in range(len(node.keys)):
                    traverse(node.children[i])
                    visit(node.keys[i][1])
                traverse(node.children[-1])

        if not self.isEmpty():
            traverse(self.root)

    def load(self, tree_dict):
        """
        Laadt een nieuwe 2-3-boom vanuit een gegeven dictionary. De huidige boom wordt verwijderd/vergeten.
        preconditie: Er moet een geldige en correcte dictionary voorstelling van de boom meegegeven.
        postconditie: De Boom wordt correct toegevoegd.
        :return: Niets
        """
        def build_tree(node_dict):
            node = Node()
            node.keys = [(key, key) for key in node_dict['root']]

            if 'children' in node_dict:
                for child_dict in node_dict['children']:
                    child_node = build_tree(child_dict)
                    child_node.parent = node
                    node.children.append(child_node)

            return node
        self.root = None

        if tree_dict:
            self.root = build_tree(tree_dict)

    def deleteItem(self, key):
        """
        Verwijdert uit de 2-3 boom tree het item met zoeksleutel searchKey.
        preconditie: Een geldige key die in de boom voorkomt.
        postconditie: Het item met key key is succesvol verwijderd, en de boom is nog steeds gebalanceert.
        :return: Success is false als er in tree geen item is met deze zoeksleutel, als het wel is gelukt True.
        """
        if self.isEmpty():
            return False
        node = self.retrieveItem_del(key)
        if node is None:
            return False
        if not node.is_leaf():
            inorder_suc = self.inorder_suc(node, key)
            index_key = self.index_key(node, key)
            temp = node.keys[index_key]
            node.keys[index_key] = inorder_suc.keys[0]
            inorder_suc.keys[0] = temp
            node = inorder_suc
        index = self.index_key(node,key)
        node.keys.pop(index)
        if len(node.keys) == 0:
            if len(self.root.keys) == 0:
                self.root = None
                return True
            self.fix(node)
        return True

    def retrieveItem_del(self, key):  # geeft knoop met key in terug
        """
        Zoekt de Node met het item dat verwijderd moet worden.
        preconditie: Een niet lege boom en een key die in de boom voorkomt.
        postconditie: Er is niets verandert aan de boom.
        :return: De Node met het item dat verwijderd moet worden, als het neit gevonden is geeft het None terug.
        """
        if self.isEmpty():
            return None
        current = self.root
        loop = False
        while loop is False:
            if len(current.keys) == 0:
                return None
            for i in current.keys:
                if key == i[0]:
                    return current
            if current.is_leaf():
                return None
            if key < current.keys[0][0] and len(current.keys) > 0:
                current = current.children[0]
            elif (len(current.keys) > 1 and key < current.keys[1][0]) or len(current.keys) == 1:
                current = current.children[1]
            else:
                current = current.children[2]

    def index_key(self, node, key):
        """
        Berekent de index van de key in de Node.
        preconditie: Een geldige Node en een key die in de Node voorkomt.
        postcondtitie: Er is niets veranderd aan de boom.
        :return: De index van de key in de Node.
        """
        teller = 0
        while teller <= 2 and node.keys[teller][0] != key:
            teller += 1
        return teller

    def inorder_suc(self, node, key):
        """
        Zoekt de correcte inorder succesor van de key die zich in een blad bevindt.
        preconditie: Er moet een correcte Node en key meegegeven worden.
        postconditie: Er is niets veranderd aan de boom.
        :return: De inorder succesor van de key.
        """
        key_index = self.index_key(node, key)
        current = node.children[key_index + 1]
        while not current.is_leaf():
            current = current.children[0]
        return current

    def fix(self, node):
        """
        Het verwijderen wordt afgerond door het verwijderen van de wortel, het herverdelen van items of het samenvoegen van knopen. Indien n geen blad is, heeft n 1 kind
        preconditie: De Node met 3 elementen wordt meegegeven.
        postconditie: De boom is correct herverdeeld.
        :return: Niets.
        """
        if self.root == node:
            new_root = self.root.children[0]
            self.root = new_root
            self.root.parent = None
            return
        parents = node.parent
        for element in parents.children:
            if len(element.keys) == 2 and -1 <= node.parent.children.index(element)-node.parent.children.index(node) <=1:
                self.redistribute(node, element)
                return
        self.merge(node)

    def redistribute(self, node, brother):
        """
        Als de Node een aangrenzende sibling heeft met 2 elementen. Herverdeel de items van sibling en parent over n, sibling en parent.
        preconditie: De lege Node wordt meegegeven net zoals de sibling met 2 elementen.
        postconditie: De Node is correct herverdeeld.
        :return: Niets
        """
        index_node = node.parent.children.index(node)
        index_brother = node.parent.children.index(brother)
        if index_node > index_brother:
            temp = brother.keys.pop(1)
            if index_node == 2:
                node.insert_key(brother.parent.keys[1])
                brother.parent.keys.pop(1)
            elif index_node == 1:
                node.insert_key(brother.parent.keys[0])
                brother.parent.keys.pop(0)
            node.parent.insert_key(temp)
            if not node.is_leaf():
                element = brother.children.pop(2)
                element.parent = node
                node.children.insert(0, element)
        elif index_node < index_brother:
            temp = brother.keys[0]
            brother.keys.pop(0)
            if index_node == 0:
                node.insert_key(brother.parent.keys[0])
                brother.parent.keys.pop(0)
            elif index_node == 1:
                node.insert_key(brother.parent.keys[1])
                brother.parent.keys.pop(1)
            node.parent.insert_key(temp)
            if not node.is_leaf():
                element = brother.children.pop(0)
                element.parent = node
                node.children.insert(2, element)
        return

    def merge(self, node):
        """
        Verplaats het juiste item van parent naar sibling.
        preconditie: Er wordt een lege Node meegegeven.
        postconditie: Het juiste item van de parent is naar de juiste sibling verplaatst.
        :return: Niets.
        """
        brothers = node.parent.children
        index_node = node.parent.children.index(node)
        if len(node.parent.keys) == 1:
            if index_node == 0:
                brother = brothers[1]
                if not node.is_leaf():
                    node.children[0].parent = brother
                    brother.children.insert(0, node.children.pop(0))
                brother.insert_key(node.parent.keys.pop(0))
            elif index_node == 1:
                brother = brothers[0]
                if not node.is_leaf():
                    node.children[0].parent = brother
                    brother.children.append(node.children.pop(0))
                brother.insert_key(node.parent.keys.pop(0))
        else:
            if index_node == 0:
                brother = brothers[1]
                if not node.is_leaf():
                    node.children[0].parent = brother
                    brother.children.insert(0, node.children.pop(0))
                brother.insert_key(node.parent.keys.pop(0))
            elif index_node == 2:
                brother = brothers[1]
                if not node.is_leaf():
                    node.children[0].parent = brother
                    brother.children.append(node.children.pop(0))
                brother.insert_key(node.parent.keys.pop(1))
            else:
                brother = brothers[0]
                if not node.is_leaf():
                    node.children[0].parent = brother
                    brother.children.append(node.children.pop(0))
                brother.insert_key(node.parent.keys.pop(0))

        node.parent.children.pop(index_node) # verwijder lege node
        if len(node.parent.keys) == 0: # als parent nu leeg is -> fix parent
            self.fix(node.parent)

    def getLenth(self):
        def _getLengthRec(node):
            if node is None:
                return 0
            if len(node.children) == 0:
                return len(node.keys)
            if len(node.children) == 1:
                return len(node.keys) + _getLengthRec(node.children[0])
            if len(node.children) == 2:
                return len(node.keys) + _getLengthRec(node.children[1]) + _getLengthRec(node.children[0])
            if len(node.children) == 3:
                return len(node.keys) + _getLengthRec(node.children[2]) + _getLengthRec(node.children[0]) + _getLengthRec(node.children[1])
        return _getLengthRec(self.root)

class TwoThreeTreeTable(TwoThreeTree):
    """
    Wrapper voor mijn 2-3 boom zodat het gebruikt kan worden als een Table.
    """
    def tableIsEmpty(self):
        return self.isEmpty()
    def tableLength(self):
        return self.getLenth()
    def tableInsert(self,newItem):
        return self.insertItem(createTreeItem(newItem[0],newItem[1]))
    def tableDelete(self,key):
        return self.deleteItem(key)
    def tableRetrieve(self,key):
        return self.retrieveItem(key)
    def traverseTableKeys(self,visit):
        return self.inorderTraverseKey(visit)
    def traverseTableValues(self,visit):
        return self.inorderTraverseValue(visit)