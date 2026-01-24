#include <iostream>
#include "Audiobestand.h"
#include "Time.h"
#include "Mediaplayer.h"

int oef3() {
    try {
        Audiobestand dreams{"Dreams", 253, "Fleetwood Mac"};
        Audiobestand smile{"Smile", 200, "Lilly Allen"};

        Mediaplayer m;
        m.add_audiobestand(&smile);
        m.add_audiobestand(&dreams);

        m.make_playlist("Chill");
        m.add_to_playlist("Chill", "Smile");
        m.add_to_playlist("Chill", "Dreams");


        m.laad_playlist("Chill");
        m.toon_huidige_playlist();
        m.speel_volgende();
        m.toon_huidige_playlist();

        m.speel_volgende();
        m.speel_volgende();

        m.toon_huidig_audiobestand();

        m.laad_playlist("Soundtrack");

    } catch (exception* e){
        cout << "Error: " << e->what() << endl;
        delete e;
    }
}
int main() {
    //Opdracht 1: debugging
    Time begin(253);
    cout<<begin.toString()<<endl;

    //Opdracht 2: code uitbreiden
    Audiobestand dreams;
    dreams.setDuur(253);
    dreams.setTitel("Dreams");
    dreams.setUitvoerder("Fleetwood Mac");

    dreams.toonInfo();
    dreams.speelAf();
    dreams.stop();
    oef3();
}
