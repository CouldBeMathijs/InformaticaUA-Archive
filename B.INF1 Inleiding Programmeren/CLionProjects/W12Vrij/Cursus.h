//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_CURSUS_H
#define W12VRIJ_CURSUS_H

#include <string>
#include <vector>
#include "Student.h"
#include <iostream>
#include "Student.h"
#include <string>

#include <vector>

#include <iostream>


class Student; // Forward declaration of Student
using namespace std;
class Cursus {
private:
    string naam;
    string afkorting;
    int studiepunten;
    vector<Student*> students;
public:
    Cursus(const string &naam, const string &afkorting, int studiepunten);

    const string &getNaam() const;

    const string &getAfkorting() const;

    int getStudiepunten() const;

    string toString() const;

    void voegToe(Student*);

    void verwijder(Student*);

    void print() const;
};


#endif //W12VRIJ_CURSUS_H
