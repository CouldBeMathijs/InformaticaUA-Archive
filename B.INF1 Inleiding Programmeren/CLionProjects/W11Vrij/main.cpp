#include <iostream>
#include "VeeltermFunctie.h"
#include "RationaleFunctie.h"
#include "Geogebra.h"
using namespace std;
int oef2() {
    RationaleFunctie f;
    f.setNaam("f");

    VeeltermFunctie teller;
    teller.setNaam("g");
    teller.setCoefficienten({1,4});
    f.setTeller(teller);

    VeeltermFunctie noemer;
    noemer.setNaam("h");
    noemer.setCoefficienten({0});
    f.setNoemer(noemer);

    noemer.setCoefficienten({1,-2,1});
    f.setNoemer(noemer);

    cout << f.toString() << endl;
    cout << f.berekenFunctiewaarde(1) << endl;
    cout << f.berekenFunctiewaarde(2) << endl;
    return 0;
}
int oef3() {
    Geogebra g;
    RationaleFunctie f;
    f.setNaam("f");

    VeeltermFunctie dummy;
    dummy.setNaam("f");
    dummy.setCoefficienten({1,4});

    VeeltermFunctie teller;
    teller.setNaam("g");
    teller.setCoefficienten({1,4});
    f.setTeller(teller);

    VeeltermFunctie noemer;
    noemer.setNaam("h");
    noemer.setCoefficienten({0});
    f.setNoemer(noemer);

    noemer.setCoefficienten({1,-2,1});
    f.setNoemer(noemer);

    g.addFunctie(&f);
    g.addFunctie(&teller);
    g.addFunctie(&noemer);
    g.addFunctie(&dummy);
    g.print();
    return 0;
}
int oef5(){
    VeeltermFunctie f;
    f.setNaam("f");
    f.setCoefficienten({1,2});
    VeeltermFunctie g;
    g.setNaam("g");
    g.setCoefficienten({1,1,1});
    cout << f.toString() << endl;
    cout << g.toString() << endl;
    f.product(g);
    cout << f.toString() << endl;
    return 0;
}
int oef4(){
    VeeltermFunctie f;
    f.setNaam("f");
    f.setCoefficienten({1,2,5});
    VeeltermFunctie g;
    g.setNaam("g");
    g.setCoefficienten({1,2,3,4,5});
    cout << f.toString() << endl;
    cout << g.toString() << endl;
    f.som(g);
    cout << f.toString() << endl;
    return 0;
}
int oef6(){
    RationaleFunctie f;
    f.setNaam("f");

    VeeltermFunctie teller;
    teller.setNaam("g");
    teller.setCoefficienten({1,4});
    f.setTeller(teller);

    VeeltermFunctie noemer;
    noemer.setNaam("h");
    noemer.setCoefficienten({1,-2,1});
    f.setNoemer(noemer);

    cout << f.toString() << endl;
    RationaleFunctie df = f.berekenAfgeleide();
    cout << df.toString() << endl;
    return 0;
}
int extraprodtest(){
    VeeltermFunctie f;
    f.setNaam("f");
    f.setCoefficienten({1,2,3});
    cout << f.toString() << endl;
    VeeltermFunctie h;
    h.setNaam("h");
    h.setCoefficienten({0,2,4});
    cout << h.toString() << endl;
    f.product(h);
    f.setNaam("f*h");
    cout << f.toString() << endl;
    return 0;
}
int main() {
//    cout << endl << "==Oefening 01==" << endl;
//    VeeltermFunctie f;
//    f.setNaam("f");
//    f.setCoefficienten({1,2,5});
//    cout << f.getGraad() << endl;
//    cout << f.toString() << endl;
//    cout << f.berekenFunctiewaarde(2) << endl;
//    cout << f.berekenFunctiewaarde(0) << endl;
//    VeeltermFunctie df = f.berekenAfgeleide();
//    cout << df.toString() << endl;
//    cout << df.berekenFunctiewaarde(2) << endl;
//    cout << endl << "==Oefening 02==" << endl;
//    oef2();
//    cout << endl << "==Oefening 03==" << endl;
//    oef3();
//    cout << endl << "==Oefening 04==" << endl;
//    oef4();
//    cout << endl << "==Oefening 05==" << endl;
//    oef5();
    cout << endl << "==Oefening 06==" << endl;
    oef6();
    cout << endl << "Extra testen voor het product" << endl;
    extraprodtest();
}
