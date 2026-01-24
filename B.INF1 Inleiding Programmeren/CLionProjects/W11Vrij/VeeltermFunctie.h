#ifndef W11VRIJ_VEELTERMFUNCTIE_H
#define W11VRIJ_VEELTERMFUNCTIE_H
#include <string>
#include <vector>
using namespace std;
class VeeltermFunctie {
protected:
    string naam;
    vector<int> coefficienten;

    vector<int> calculateProduct(const vector<int>& a, const vector<int>& b) const;

public:
    const string &getNaam() const;

    void setNaam(const string &naam);

    int getGraad() const;

    void setCoefficienten(const vector<int> &coefficienten);

    string toString() const;

    double berekenFunctiewaarde(double x);

    VeeltermFunctie berekenAfgeleide();

    VeeltermFunctie();

    explicit VeeltermFunctie(const string &naam);

    VeeltermFunctie(const string &naam, const vector<int> &coefficienten);

    const vector<int> &getCoefficienten() const;

    void som(const VeeltermFunctie &opteller);

    void verschil(const VeeltermFunctie &aftrekker);

    void product(const VeeltermFunctie &tevermenigvuldigen);

};


#endif //W11VRIJ_VEELTERMFUNCTIE_H
