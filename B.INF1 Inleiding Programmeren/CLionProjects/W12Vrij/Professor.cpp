//
// Created by mathijs on 12/18/24.
//

#include "Professor.h"
#include <iostream>
#include "Cursus.h"
#include "Assistent.h"
Professor::Professor(const string &voornaam, const string &achternaam) : Lesgever(voornaam,achternaam){}

string Professor::toString() const {
    return "Professor " + Lesgever::toString();
}


void Professor::voegAssistentToe(Assistent * assistent) {
    assistenten.push_back(assistent);
}

void Professor::showHomepage() const {
    Lesgever::showHomepage();
    string assistentenstring = " (";
    bool first = true;
    for (auto& assistent : assistenten) {
        if (!first) {
            assistentenstring += ", ";
        }
        assistentenstring += assistent->toString();
        first = false;
    }
    assistentenstring += ")";
    for (auto& cursus : cursussen) {
        cout << "- " << cursus->toString() << assistentenstring << endl;
    }
}


