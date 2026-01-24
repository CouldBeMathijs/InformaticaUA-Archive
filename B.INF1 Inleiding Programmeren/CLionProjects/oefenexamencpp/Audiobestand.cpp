#include "Audiobestand.h"
#include <iostream>

Audiobestand::Audiobestand() {
}

long Audiobestand::getDuur() const {
    return duur;
}

const string & Audiobestand::getTitel() const {
    return titel;
}

const string & Audiobestand::getUitvoerder() const {
    return uitvoerder;
}

void Audiobestand::setDuur(long duur) {
    Audiobestand::duur = duur;
}

void Audiobestand::setTitel(const string &titel) {
    Audiobestand::titel= titel;
}

void Audiobestand::setUitvoerder(const string &uitvoerder) {
    Audiobestand::uitvoerder = uitvoerder;
}

void Audiobestand::toonInfo() {
    cout<<toString()<<endl;
}

string Audiobestand::toString() {
    long uren = duur/3600;
    long minuten = duur/60;
    long seconden = duur % 60;
    string tijd = to_string(uren) + ": " + to_string(minuten) + ": " + to_string(seconden);
    return getTitel() + " - " + getUitvoerder() + " - " + tijd;
}

void Audiobestand::stop() {
    cout << "Stoppen van " << getTitel() << endl;
    return;
}

void Audiobestand::speelAf() {
    cout << "Afspelen van " << getTitel() << endl;
    return;
}

Audiobestand::Audiobestand(const string &titel, long duur, const string &uitvoerder) : titel(titel), duur(duur),
                                                                                       uitvoerder(uitvoerder) {}
