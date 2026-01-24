//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//  !!!       Enkel de filename mag je aanpassen      !!!
//

#include <SFML/Graphics.hpp>
#include "Game.h"

int main() { //NEBM
    sf::RenderWindow window(sf::VideoMode(700, 700), "Game");
    Game game = Game(&window);
    game.loadMap("resources/mapdefault.txt");
    game.update();
    return 0;
}

//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//  !!!       Enkel de filename mag je aanpassen      !!!
//



//int main() { //NEBM
//    sf::RenderWindow window(sf::VideoMode(700, 700), "Game");
//    Game game = Game(&window);
//    game.loadMap("resources/mapdefault.txt");
//    game.update();
//    Room* room = game.getCurrentRoom();
//    sf::Event event = sf::Event();
//    game.update();
//    event.key.code = sf::Keyboard::Up;
//    room->update(&event);
//    game.update();
//    event.key.code = sf::Keyboard::Left;
//    room->update(&event);
//    game.update();
//    event.key.code = sf::Keyboard::Down;
//    room->update(&event);
//    event.key.code = sf::Keyboard::Right;
//    room->update(&event);
//    return 0;
//}