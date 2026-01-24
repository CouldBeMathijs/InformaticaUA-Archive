#ifndef WEEK12_1_AUDIOBESTAND_H
#define WEEK12_1_AUDIOBESTAND_H

#include "string"
using namespace std;

class Audiobestand {

private:
    string titel;
    long duur; //in seconden
    string uitvoerder;

public:
    Audiobestand();

    Audiobestand(const string &titel, long duur, const string &uitvoerder);

    const string& getTitel() const;
    void setTitel(const string& titel);
    long getDuur() const;
    void setDuur(long duur);
    const string& getUitvoerder() const;
    void setUitvoerder(const string& uitvoerder);

    string toString();
    void toonInfo();
    void speelAf();
    void stop();
};


#endif //WEEK12_1_AUDIOBESTAND_H
