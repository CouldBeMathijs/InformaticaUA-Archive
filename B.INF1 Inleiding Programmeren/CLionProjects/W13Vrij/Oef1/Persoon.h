//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_PERSOON_H
#define W12VRIJ_PERSOON_H
#include <string>
using namespace std;

class Persoon {
private:
    string voornaam;
    string achternaam;
public:
    Persoon(const string &voornaam, const string &achternaam);

    const string &getVoornaam() const;

    const string &getAchternaam() const;

    virtual string toString() const;

    virtual void showHomepage() const = 0;

    void showWebsite();
};


#endif //W12VRIJ_PERSOON_H
