//
// Created by mathijs on 12/18/24.
//

#include "Persoon.h"
#include <iostream>
Persoon::Persoon(const string &voornaam, const string &achternaam) : voornaam(voornaam), achternaam(achternaam) {}

const string &Persoon::getVoornaam() const {
    return voornaam;
}

const string &Persoon::getAchternaam() const {
    return achternaam;
}

string Persoon::toString() const {
    return getVoornaam() + " " + getAchternaam();
}

void Persoon::showHomepage() const {
    cout << this->toString() << " is een persoon op de UAntwerpen" << endl;
}

