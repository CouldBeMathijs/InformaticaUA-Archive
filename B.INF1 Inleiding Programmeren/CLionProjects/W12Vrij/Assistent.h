//
// Created by mathijs on 12/18/24.
//

#ifndef W12VRIJ_ASSISTENT_H
#define W12VRIJ_ASSISTENT_H

#include "Lesgever.h"
#include <string>
class Professor;
class Assistent : public Lesgever{
private:
    Professor* begeleider;
public:
    Professor *getBegeleider() const;

    void setBegeleider(Professor *begeleider);

    Assistent(const string &voornaam, const string &achternaam);

    void showHomepage() const override;
};


#endif //W12VRIJ_ASSISTENT_H
