//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_PROFESSOR_H
#define W12VRIJ_PROFESSOR_H

#include <string>
#include <vector>
#include "Lesgever.h"

using namespace std;
class Cursus;
class Assistent;
class Professor : public Lesgever{
private:
    vector<Assistent*> assistenten;
public:
    Professor(const string &achternaam, const string &voornaam);

    string toString() const;

    void voegAssistentToe(Assistent*);

    void showHomepage() const override;
};


#endif //W12VRIJ_PROFESSOR_H
