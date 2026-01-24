#include <iostream>
#include <string>
using namespace std;
struct User {
    string firstname;
    string lastname;
};
struct BST {
    User* value = nullptr;
    BST* leftTree = nullptr;
    BST* rightTree = nullptr;
    int key = 0;
};
BST* createBST() {
    auto * bst = new BST;
    return bst;
}
bool isEmpty(BST* bst) {
    return (bst == nullptr);
}
void destroy(BST* tree) {
    if (tree != nullptr) {
        destroy(tree->leftTree);
        destroy(tree->rightTree);
        delete tree->value;
        delete tree;
    }
}
string save(BST* tree) {
    //Als de boom leeg is, geef een lege dict. terug
    if (tree == nullptr) {
        return "{}";
    }
    //Initialiseer result met de root
    string result = "{'root': " + to_string(tree->key);
    //Roep recursief deze functie op voor de linker, en dan voor de rechter deelboom
    if (tree->leftTree != nullptr) {
        result += ", 'left': " + save(tree->leftTree);
    }
    if (tree->rightTree != nullptr) {
        result += ", 'right': " + save(tree->rightTree);
    }
    result += "}";
    return result;
}
void insert(BST*& tree, int key, User* value) {
    if (tree == nullptr) {
        // Als de boom leeg is, maak een nieuwe knoop aan
        tree = new BST;
        tree->key = key;
        tree->value = value;
        tree->leftTree = nullptr;
        tree->rightTree = nullptr;
    } else if (key == tree->key) {
        // Als de key al bestaat, overschrijf de value
        delete tree->value;
        tree->value = value;
    } else if (key < tree->key) {
        // Ga naar de linker subboom
        insert(tree->leftTree, key, value);
    } else {
        // Ga naar de rechter subboom
        insert(tree->rightTree, key, value);
    }
}
void visit(User* user, int key) {
    if (user != nullptr) {
        cout << key <<" " << user->firstname << " " << user->lastname << endl;
    } else {
        cout << "User bestaat niet" << endl;
    }
}
User * find(BST* tree, int key) {
    if (tree == nullptr) {
        return nullptr;
    }
    if (tree->key == key) {
        return tree->value;
    }
    if (key < tree->key) {
        return find(tree->leftTree, key);
    }
    return find(tree->rightTree, key);
}
void preorder(BST* tree) {
    if (tree != nullptr) {
        visit(tree->value,tree->key);
        preorder(tree->leftTree);
        preorder(tree->rightTree);
    }
}
void inorder(BST* tree) {
    if (tree != nullptr) {
        inorder(tree->leftTree);
        visit(tree->value,tree->key);
        inorder(tree->rightTree);
    }
}
void postorder(BST* tree) {
    if (tree != nullptr) {
        postorder(tree->leftTree);
        postorder(tree->rightTree);
        visit(tree->value,tree->key);
    }
}
BST* rotateLeft(BST* tree) {
    // Controleer of de rotatie mogelijk is
    if (tree == nullptr || tree->rightTree == nullptr) {
        // Als de boom leeg is of er geen rechterkind is, kan er niet geroteerd worden
        return tree;
    }
    // Bewaar het rechterkind in een tijdelijke variabele
    BST* newRoot = tree->rightTree;
    // Voer de rotatie uit
    tree->rightTree = newRoot->leftTree; // Het linkerkind van de nieuwe root wordt het rechterkind van de oude root
    newRoot->leftTree = tree;            // De oude root wordt het linker kind van de nieuwe root
    // Geef de nieuwe root terug
    return newRoot;
}
BST* rotateRight(BST* tree) {
    // Controleer of de rotatie mogelijk is
    if (tree == nullptr || tree->leftTree == nullptr) {
        // Als de boom leeg is of er geen linkerkind is, kan er niet geroteerd worden
        return tree;
    }
    // Bewaar het linkerkind in een tijdelijke variabele
    BST* newRoot = tree->leftTree;
    // Voer de rotatie uit
    tree->leftTree = newRoot->rightTree; // Het rechter kind van de nieuwe root wordt het linker kind van de oude root
    newRoot->rightTree = tree;            // De oude root wordt het rechter kind van de nieuwe root
    // Geef de nieuwe root terug
    return newRoot;
}
int height(BST* tree) {
    // De hoogte van een boom is de hoogte van de linkersubtree + rechtersubtree + 1
    if (tree == nullptr) {
        return 0;
    }
    return 1 + max(height(tree->leftTree), height(tree->rightTree));
}
BST* balance(BST* tree) {
    if (tree == nullptr) {
        return tree;
    }
    // Bereken de hoogte van de linker en rechter subtrees
    int leftHeight = height(tree->leftTree);
    int rightHeight = height(tree->rightTree);
    // Balanceren als de boom alleen rechter kinderen heeft
    while (rightHeight > leftHeight + 1) {
        tree = rotateLeft(tree);
        rightHeight = height(tree->rightTree);
        leftHeight = height(tree->leftTree);
    }
    // Balanceren als de boom alleen linker kinderen heeft
    while (leftHeight > rightHeight + 1) {
        tree = rotateRight(tree);
        leftHeight = height(tree->leftTree);
        rightHeight = height(tree->rightTree);
    }
    // Balanceren van de linker en rechter deelbomen
    tree->leftTree = balance(tree->leftTree);
    tree->rightTree = balance(tree->rightTree);
    return tree;
}
#include <fstream>

int main() {
    BST* bst = createBST();
    cout << find(bst,15) << endl;
    // momenteel manueel omdat er nog geen insert is
    bst = new BST;
    bst->key = 15;
    bst->value = new User;
    bst->value->firstname="John";
    bst->value->lastname="15";
    bst->leftTree = new BST;
    bst->leftTree->key = 5;
    bst->leftTree->value = new User;
    bst->leftTree->value->firstname="John";
    bst->leftTree->value->lastname="15";
    cout << find(bst,10) << endl;
    cout << find(bst,15)->firstname << endl;
    destroy(bst);
    return 0;
}
