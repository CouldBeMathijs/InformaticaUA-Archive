//
// Created by mathijs on 12/18/24.
//

#include "Student.h"
#include "Cursus.h"
Student::Student(const string &voornaam, const string &achternaam, const string &rolnummer) : Persoon(voornaam, achternaam),
                                                                                              rolnummer(rolnummer) {}

const string &Student::getRolnummer() const {
    return rolnummer;
}

string Student::toString() const {
    return Persoon::toString() + " (" + getRolnummer() + ")";
}

int Student::getStudiepunten() {
    int out = 0;
    for (auto& cursus:cursussen) {
        out += cursus.second->getStudiepunten();
    }
    return out;
}

bool Student::schrijfUit(Cursus * oldCursus) {
    for (auto& cursus:cursussen) {
        if (cursus.second == oldCursus) {
            oldCursus->verwijder(this);
            cursussen.erase(oldCursus->getAfkorting());
            return true;
        }
    }
    return false;
}

bool Student::schrijfIn(Cursus * cursus) {
    if (this->getStudiepunten() + cursus->getStudiepunten() > 60) {
        return false;
    }
    cursussen[cursus->getAfkorting()] = cursus;
    cursus->voegToe(this);
    return true;
}

void Student::showHomepage() const {
    cout << this->toString() << " is een student van de UAntwerpen en volgt" << endl;
    for (auto& cursus:cursussen){
        cout << "- " << cursus.second->toString() << endl;
    }
}



