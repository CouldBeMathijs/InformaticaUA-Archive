#ifndef W11VRIJ_GEOGEBRA_H
#define W11VRIJ_GEOGEBRA_H
#include <map>
#include "RationaleFunctie.h"
#include "VeeltermFunctie.h"
#include <string>
using namespace std;
class Geogebra {
private:
    map<string, VeeltermFunctie*> veeltermfuncties;
    map<string, RationaleFunctie*> rationalefuncties;
public:
    bool addFunctie(RationaleFunctie*);
    bool addFunctie(VeeltermFunctie*);
    void print() const;
};


#endif //W11VRIJ_GEOGEBRA_H
