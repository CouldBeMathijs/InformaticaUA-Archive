#include <iostream>
using namespace std;
#include "Oef1/Student.h"
#include "Oef1/Cursus.h"
#include "Oef1/Professor.h"
#include "Oef1/Assistent.h"
#include "Oef2/Movie.cpp"
int Oef1() {
    // student
    Student john("John", "Doe", "12345");
    Student jane("Jane", "Doe", "54321");
    vector<Persoon*> universiteit;
    universiteit.push_back(&john);
    universiteit.push_back(&jane);

    // course
    Cursus ip("IP", "Inleiding programmeren", 9);
    Cursus gas("GAS", "Gegevensabstractie en -structuren", 6);
    Cursus mc("MC", "Monster Cursus", 56);

    vector<Cursus*> cursussen;
    cursussen.push_back(&ip);
    cursussen.push_back(&gas);
    cursussen.push_back(&mc);

    // inschrijven
    john.schrijfIn(&ip);
    john.schrijfIn(&gas);
    john.schrijfIn(&mc);
    jane.schrijfIn(&ip);

    // professor
    Professor toon("Toon", "Calders");
    toon.geeft(&ip);
    Professor els("Els", "Laenens");
    els.geeft(&gas);
    universiteit.push_back(&toon);
    universiteit.push_back(&els);

    // assistenten
    Assistent tom("Tom", "Hofkens");
    tom.setBegeleider(&toon);
    Assistent daphne("Daphne", "Lenders");
    daphne.setBegeleider(&toon);
    tom.geeft(&ip);
    tom.geeft(&gas);
    daphne.geeft(&ip);
    toon.voegAssistentToe(&daphne);
    toon.voegAssistentToe(&tom);
    els.voegAssistentToe(&tom);
    universiteit.push_back(&tom);
    universiteit.push_back(&daphne);

    // print alles
    for(Persoon* member : universiteit){
        cout << member->toString() << endl;
    }
    for(Persoon* member : universiteit){
        member->showWebsite();
    }
    return 0;
}
int Oef2() {
    MovieProvider* netflix = new Netflix(10);
    netflix->addMovie("Inception");
    netflix->setRating("Inception", 0.9);
    netflix->print();
    delete netflix;
    return 0;
}
int main() {
    cout << endl << "==Oefening 1==" << endl;
    Oef1();
    cout << endl << "==Oefening 2==" << endl;
    Oef2();
//    cout << endl << "==Oefening 3==" << endl;
//    Oef3();
//    cout << endl << "==Oefening 4==" << endl;
//    Oef4();
//    return 0;
}
