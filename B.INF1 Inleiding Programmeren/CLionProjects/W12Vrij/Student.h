//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_STUDENT_H
#define W12VRIJ_STUDENT_H
#include <string>
#include <map>
#include "Persoon.h"
class Cursus;
using namespace std;

class Student : public Persoon{
private:
    string voornaam;
    string achternaam;
    string rolnummer;
    map<string, Cursus*> cursussen;
public:
    Student(const string &voornaam, const string &achternaam, const string &rolnummer);

    const string &getRolnummer() const;

    string toString() const;

    int getStudiepunten();

    bool schrijfIn(Cursus*);

    bool schrijfUit(Cursus*);

    void showHomepage() const override;
};


#endif //W12VRIJ_STUDENT_H
