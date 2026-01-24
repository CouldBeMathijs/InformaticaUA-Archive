#ifndef W11VRIJ_RATIONALEFUNCTIE_H
#define W11VRIJ_RATIONALEFUNCTIE_H

#include <string>
#include "VeeltermFunctie.h"
using namespace std;
class RationaleFunctie {
private:
    string naam;
    VeeltermFunctie teller;
    VeeltermFunctie noemer;
public:
    const string &getNaam() const;

    void setNaam(const string &naam);

    void setTeller(const VeeltermFunctie &teller);

    void setNoemer(const VeeltermFunctie &noemer);

    string toString() const;

    double berekenFunctiewaarde(double x);

    RationaleFunctie berekenAfgeleide();
};


#endif //W11VRIJ_RATIONALEFUNCTIE_H
