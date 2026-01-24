//
// Created by mathijs on 12/18/24.
//

#include "Lesgever.h"
#include <iostream>
#include "Cursus.h"
void Lesgever::geeft(Cursus *cursus) {
    cursussen.push_back(cursus);
}

Lesgever::Lesgever(const string &voornaam, const string &achternaam) : Persoon(voornaam, achternaam) {}

void Lesgever::showHomepage() const {
    cout << this->toString() << " is een lesgever op de UAntwerpen en geeft" << endl;
}
