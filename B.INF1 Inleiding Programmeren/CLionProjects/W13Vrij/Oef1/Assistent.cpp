//
// Created by mathijs on 12/18/24.
//

#include "Assistent.h"
#include "Cursus.h"
#include "Professor.h"
#include <iostream>
Professor *Assistent::getBegeleider() const {
    return begeleider;
}

void Assistent::setBegeleider(Professor *begeleider) {
    Assistent::begeleider = begeleider;
}

Assistent::Assistent(const string &voornaam, const string &achternaam) : Lesgever(voornaam, achternaam) {}

void Assistent::showHomepage() const {
    Lesgever::showHomepage();
    for (auto& cursus: cursussen) {
        cout << "- " << cursus->toString() << " (" << begeleider->toString() << ")" << endl;
    }
}
