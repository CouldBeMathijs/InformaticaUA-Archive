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

void Persoon::showWebsite() {
    cout << "Welkom op de Universiteit Antwerpen! U bent terecht gekomen op de homepage van " << this->toString() << endl <<
            "=====================" << endl;
    this->showHomepage();
}
