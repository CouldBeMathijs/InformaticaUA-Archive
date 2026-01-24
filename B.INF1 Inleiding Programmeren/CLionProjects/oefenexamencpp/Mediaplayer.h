//
// Created by mathijs on 1/11/25.
//

#ifndef OEFENEXAMENCPP_MEDIAPLAYER_H
#define OEFENEXAMENCPP_MEDIAPLAYER_H

#include <string>

using namespace std;
class Audiobestand {};
class Mediaplayer {
    void add_audiobestand(Audiobestand *);
    void make_playlist(string naam);
    void add_to_playlist(string playlistnaam, string bestand);
    void laad_playlist(string playlist_naam);
    void toon_huidige_playlist();
    void speel_volgende();
    void toon_huidig_audiobestand();

};


#endif //OEFENEXAMENCPP_MEDIAPLAYER_H
