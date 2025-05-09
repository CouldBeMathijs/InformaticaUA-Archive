//
// Created by Tim Apers on 29/10/2024.
//

#ifndef GAME_H
#define GAME_H

#include "Room.h"
#include <map>

using namespace std;
class Game {
public:
    // Deze functies niet aanpassen!
    Game(sf::RenderWindow* window);
    void update();
    std::vector<Room *> getRooms() { return rooms; }
    Room* getCurrentRoom() { return currentRoom; }

    // Deze functies zal je zelf verder moeten aanvullen
    void loadMap(const std::string& filename);
    void setCurrentRoom();

private:
    sf::RenderWindow* window;
    std::vector<Room*> rooms;
    Room* currentRoom = nullptr;
    map<Position, Room*> matrix = {};
    void drawEntity(int ch, Room *curRoom, Position entityposition);

    void changeToRoom(Room *newRoom);
};



#endif //GAME_H
