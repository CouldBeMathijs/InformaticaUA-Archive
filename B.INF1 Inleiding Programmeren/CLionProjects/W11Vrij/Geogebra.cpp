//
// Created by mathijs on 12/9/24.
//

#include "Geogebra.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
bool Geogebra::addFunctie(RationaleFunctie * newfunct) {
    for (const auto& funct : rationalefuncties) {
        if (funct.first == newfunct->getNaam()){
            return false;
        }
    }
    for (const auto& funct : veeltermfuncties) {
        if (funct.first == newfunct->getNaam()){
            return false;
        }
    }
    // Anders
    rationalefuncties[newfunct->getNaam()] = newfunct;
    return true;
}

bool Geogebra::addFunctie(VeeltermFunctie * newfunct) {
    for (const auto& funct : rationalefuncties) {
        if (funct.first == newfunct->getNaam()){
            return false;
        }
    }
    for (const auto& funct : veeltermfuncties) {
        if (funct.first == newfunct->getNaam()){
            return false;
        }
    }
    // Anders
    veeltermfuncties[newfunct->getNaam()] = newfunct;
    return true;
}

void Geogebra::print() const {
    for (const auto& funct: veeltermfuncties){
        cout << funct.second->VeeltermFunctie::toString() << endl;
    }
    for (const auto& funct: rationalefuncties){
        cout << funct.second->RationaleFunctie::toString() << endl;
    }
}


