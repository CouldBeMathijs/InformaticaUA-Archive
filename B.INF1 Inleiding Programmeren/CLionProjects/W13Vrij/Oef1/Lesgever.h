//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_LESGEVER_H
#define W12VRIJ_LESGEVER_H


#include <vector>
#include "Persoon.h"
class Cursus;
class Lesgever : public Persoon{
protected:
    vector<Cursus*> cursussen;
public:
    void geeft(Cursus* cursus);

    Lesgever(const string &voornaam, const string &achternaam);

    void showHomepage() const override;
};


#endif //W12VRIJ_LESGEVER_H
