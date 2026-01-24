#include "RationaleFunctie.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <string>
#include "VeeltermFunctie.h"
using namespace std;
const string &RationaleFunctie::getNaam() const {
    return naam;
}

void RationaleFunctie::setNaam(const string &naam) {
    RationaleFunctie::naam = naam;
}

void RationaleFunctie::setTeller(const VeeltermFunctie &teller) {
    RationaleFunctie::teller = teller;
}

void RationaleFunctie::setNoemer(const VeeltermFunctie &noemer) {

    if (noemer.getCoefficienten()[0] == 0 && noemer.getCoefficienten().size() == 1){
        cout << "Ongeldige noemer" << endl;
        return;
    }
    RationaleFunctie::noemer = noemer;
}

string RationaleFunctie::toString() const {
    ostringstream out;
    regex pattern(R"(\\ = )");
    string noemerstring = noemer.toString();
    // Find the position of " = "
    size_t pos = noemerstring.find(" = ");

    // If found, remove everything before and including " = "
    if (pos != std::string::npos) {
        noemerstring.erase(0, pos + 3); // +3 to remove the " = "
    }
    string tellerstring = teller.toString();
    // Find the position of " = "
    pos = tellerstring.find(" = ");

    // If found, remove everything before and including " = "
    if (pos != std::string::npos) {
        tellerstring.erase(0, pos + 3); // +3 to remove the " = "
    }
    out << naam << "(x) = " << "(" << tellerstring << ") / (" << noemerstring << ")";
    return out.str();
}

double RationaleFunctie::berekenFunctiewaarde(double x) {
    double noemerWaarde = noemer.berekenFunctiewaarde(x);
    if (noemerWaarde == 0) {
        cout << "Deze waarde ligt buiten het domein van de functie" << endl;
        return 0;
    }
    double tellerWaarde = teller.berekenFunctiewaarde(x);
    return tellerWaarde / noemerWaarde;
}
RationaleFunctie RationaleFunctie::berekenAfgeleide() {
    // Access numerator (f) and denominator (g)
    VeeltermFunctie& f = this->teller;  // f(x)
    VeeltermFunctie& g = this->noemer;  // g(x)
    auto f_accent = f.berekenAfgeleide();  // f'(x)
    auto g_accent = g.berekenAfgeleide();  // g'(x)

    // Calculate f'(x) * g(x)
    VeeltermFunctie f_accent_g = f_accent;
    f_accent_g.product(g);

    // Calculate f(x) * g'(x)
    VeeltermFunctie f_g_accent = f;
    f_g_accent.product(g_accent);


    // Subtract f(x) * g'(x) from f'(x) * g(x)
    VeeltermFunctie numerator = f_accent_g;
    numerator.verschil(f_g_accent);  // Perform addition with negated term (this subtracts)

    // Denominator: g(x)^2
    VeeltermFunctie denominator = g;
    denominator.product(g);  // Square the denominator

    // Construct the rational derivative
    RationaleFunctie out;
    out.setTeller(numerator);
    out.setNoemer(denominator);
    out.setNaam(this->naam + "'");
    return out;
}
