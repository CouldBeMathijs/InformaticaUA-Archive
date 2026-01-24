//
// Created by mathijs on 12/18/24.
//

#include "Cursus.h"
#include "Student.h"
Cursus::Cursus(const string &naam, const string &afkorting, int studiepunten) : naam(naam), afkorting(afkorting),
                                                                                studiepunten(studiepunten) {}

const string &Cursus::getNaam() const {
    return naam;
}

const string &Cursus::getAfkorting() const {
    return afkorting;
}

int Cursus::getStudiepunten() const {
    return studiepunten;
}

string Cursus::toString() const {
    return getAfkorting() + " - " + getNaam() + " (" + to_string(getStudiepunten()) + ")";
}

void Cursus::voegToe(Student * newStudent) {
    students.push_back(newStudent);
}

void Cursus::verwijder(Student *oldStudent) {
    for (size_t i = 0; i < students.size(); /* no increment here */) {
        if (students[i] == oldStudent) {
            students.erase(students.begin() + i);
        } else {
            ++i;
        }
    }
}

void Cursus::print() const {
    cout << this->toString() << endl;
    for (auto& student: students) {
        cout << '\t' << student->toString() << endl;
    }
}

