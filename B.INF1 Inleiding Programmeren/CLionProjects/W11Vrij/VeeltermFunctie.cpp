#include "VeeltermFunctie.h"
#include <sstream>
#include <cmath>
using namespace std;
const string &VeeltermFunctie::getNaam() const {
    return naam;
}

void VeeltermFunctie::setNaam(const string &naam) {
    VeeltermFunctie::naam = naam;
}
int VeeltermFunctie::getGraad() const{
    return VeeltermFunctie::coefficienten.size() - 1;
}

void VeeltermFunctie::setCoefficienten(const vector<int> &coefficienten) {
    VeeltermFunctie::coefficienten = coefficienten;
}

#include <sstream> // Voor ostringstream
#include <vector>
#include <string>

#include <sstream> // Voor ostringstream
#include <vector>
#include <string>
#include <cmath> // Voor abs

string VeeltermFunctie::toString() const {
    ostringstream oss;
    oss << naam << "(x) = ";

    // Loop door de coëfficiënten en bouw de string op
    for (int i = getGraad(); i >= 0; --i) {
        int coeff = coefficienten[i];
        if (coeff != 0) { // Alleen niet-nul coëfficiënten toevoegen
            if (i < getGraad()) { // Voeg een '+' toe als het niet de eerste term is
                oss << (coeff > 0 ? " + " : " - ");
            } else {
                oss << (coeff > 0 ? "" : "-");

            }
            // Voeg de absolute waarde van de coëfficiënt toe
            oss << abs(coeff);
            // Voeg de macht van x toe, indien nodig
            if (i > 0) {
                oss << " x";
                if (i > 1) {
                    oss << "^" << i; // Voeg de macht toe als het groter is dan 1
                }
            }
        }
    }

    return oss.str(); // Geef de geconstrueerde string terug
}

double VeeltermFunctie::berekenFunctiewaarde(double x) {
    double out = 0;
    for (int i = 0; i < coefficienten.size(); i++){
        out += coefficienten[i] * pow(x, i);
    }
    return out;
}

VeeltermFunctie VeeltermFunctie::berekenAfgeleide(){
    string newnaam = naam + "'";
    vector<int> newcoef;

    for (size_t i = 1; i < coefficienten.size(); ++i) {
        newcoef.push_back(coefficienten[i] * i);
    }
    return VeeltermFunctie(newnaam, newcoef);
}

VeeltermFunctie::VeeltermFunctie() {}

VeeltermFunctie::VeeltermFunctie(const string &naam) : naam(naam) {}


VeeltermFunctie::VeeltermFunctie(const string &naam, const vector<int> &coefficienten) : naam(naam),
                                                                                         coefficienten(coefficienten) {}

const vector<int> &VeeltermFunctie::getCoefficienten() const {
    return coefficienten;
}

void VeeltermFunctie::som(const VeeltermFunctie& opteller) {
    // Een paar referenties om het leesbaarder te maken
    const vector<int>& optellercoef = opteller.getCoefficienten();
    const size_t grootste = max(optellercoef.size(), this->coefficienten.size());

    // Vergroot de hoofdveeltermfunctie indien nodig
    if (this->coefficienten.size() < grootste) {
        this->coefficienten.resize(grootste, 0); // Vul de nieuwe coëfficiënten met 0'en
    }

    // En tel opteller er bij op
    for (int i = 0; i < optellercoef.size(); i++) {
        this->coefficienten[i] += optellercoef[i];
    }
}
void VeeltermFunctie::verschil(const VeeltermFunctie& aftrekker) {
    // Get references to the coefficients for readability
    const std::vector<int>& aftrekkercoef = aftrekker.getCoefficienten();
    size_t grootste = std::max(aftrekkercoef.size(), this->coefficienten.size());

    // Ensure the current coefficients vector is large enough
    this->coefficienten.resize(grootste, 0); // Fill new entries with 0s if resized

    // Subtract the coefficients
    for (size_t i = 0; i < aftrekkercoef.size(); ++i) {
        this->coefficienten[i] -= aftrekkercoef[i];
    }
}
void VeeltermFunctie::product(const VeeltermFunctie& tevermenigvuldigen) {
    // Get the coefficients of the multiplier polynomial
    const std::vector<int>& multiplierCoef = tevermenigvuldigen.getCoefficienten();

    // Calculate the product of the two polynomials
    std::vector<int> result = calculateProduct(this->coefficienten, multiplierCoef);

    // Update the current polynomial with the result
    this->coefficienten = std::move(result); // Use move to optimize
}

std::vector<int> VeeltermFunctie::calculateProduct(const std::vector<int>& a, const std::vector<int>& b) const {
    // Allocate space for the resulting coefficients (degree a + degree b)
    std::vector<int> result(a.size() + b.size() - 1, 0);

    // Multiply and accumulate terms
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            // The index in the result is i + j, not a[i] + j
            result[i + j] += a[i] * b[j];
        }
    }

    return result;
}

