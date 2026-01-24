//
// Created by Tim Apers on 29/10/2024.
//

#include "Game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
enum {
    ROOMSIZE = 7,
    TILESIZE = 100
};
Position roundDownToMultiple(Position pos, unsigned int multiple) {
    if (multiple == 0) {
        throw std::invalid_argument("Multiple must be greater than 0");
    }

    Position roundedPos;
    roundedPos.x = (pos.x / multiple) * multiple;
    roundedPos.y = (pos.y / multiple) * multiple;

    return roundedPos;
}
Game::Game(sf::RenderWindow* window) { //NEBM
    this->window = window;
}

void Game::update() { //NEBM
    setCurrentRoom();
    this->window->clear();
    if (currentRoom) { currentRoom->render(this->window); }
    this->window->display();
    while (window->isOpen()) {
        sf::Event event{};

        // Wait for event to fire
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::KeyPressed) {
                currentRoom->update(&event);
                setCurrentRoom();
                this->window->clear();
                currentRoom->render(this->window);
                this->window->display();
            }
        }
    }
}

void Game::loadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    vector<vector<char>> mapData;
    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        mapData.push_back(row);
    }
    file.close();
    Position entityposition;

    auto everything = new Room();
    for (const auto & line: mapData) {
        for (auto ch: line) {
            drawEntity(ch, everything, entityposition);
            entityposition.x += TILESIZE;
        }
        entityposition.y += TILESIZE;
        entityposition.x = 0;
    }
    const auto& entities = everything->getEntities();
    // Create the correct amount of rooms
    Position maxposition;
    for (auto entity:entities) {
        if (entity->getPosition().x > maxposition.x){
            maxposition.x = entity->getPosition().x;
        }
        if (entity->getPosition().y > maxposition.y){
            maxposition.y = entity->getPosition().y;
        }
    }
    for (unsigned int x = 0; x <= maxposition.x; x += TILESIZE) {
        for (unsigned int y = 0; y <= maxposition.y; y += TILESIZE) {
            Position roomPosition = roundDownToMultiple(Position(x,y), TILESIZE * ROOMSIZE);
            if (matrix.count(roomPosition) > 0) {
             // De kamer bestaat al, dus voeg hem niet opnieuw toe
            } else {
                auto newRoom = new Room;
                matrix[roomPosition] = newRoom;
                rooms.push_back(newRoom);            }

        }
    }
    // Now add each entity to the correct room
    for (auto entity: entities) {
        auto entityRoom = matrix[roundDownToMultiple(Position(entity->getPosition()), TILESIZE * ROOMSIZE) ];
        entityRoom->addEntity(entity);
    }
    delete everything;
    for (auto room: rooms) {
        for (auto entity: room->getEntities()){
            if (dynamic_cast<Player*>(entity)) {
                currentRoom = room;
                currentRoom->playerpointer = entity;
            }
        }
    }
}


void Game::drawEntity(int ch, Room* curRoom, Position entityposition){
    if (ch == '#') {
        auto wall = new Wall(entityposition);
        wall->setSprite("resources/wall.png");
        curRoom->addEntity(wall);
    } else if (ch == '_') {
        auto floor = new Floor(entityposition);
        floor->setSprite("resources/floor.png");
        curRoom->addEntity(floor);
    } else if (ch == '%') {
        //Also add a floor, underneath
        auto floor = new Floor(entityposition);
        floor->setSprite("resources/floor.png");
        curRoom->addEntity(floor);
        auto enemy = new Enemy(entityposition);
        enemy->setSprite("resources/enemy.png");
        curRoom->addEntity(enemy);
    } else if (ch == '!') {
        //Also add a floor, underneath
        auto floor = new Floor(entityposition);
        floor->setSprite("resources/floor.png");
        curRoom->addEntity(floor);
        auto weapon = new Weapon(entityposition);
        weapon->setSprite("resources/weapon.png");
        curRoom->addEntity(weapon);
    } else if (ch == '@') {
        //Also add a floor, underneath
        auto floor = new Floor(entityposition);
        floor->setSprite("resources/floor.png");
        curRoom->addEntity(floor);
        auto player = new Player(entityposition);
        player->setSprite("resources/player.png");
        curRoom->addEntity(player);
    }
}
void Game::setCurrentRoom() {
    for (auto room : rooms) {
        // Voor elke entiteit in elke kamer
        for (auto entity : room->getEntities()) {
            // Als het de speler is
            if (dynamic_cast<Player*>(entity)) {
                changeToRoom(matrix[roundDownToMultiple(Position(entity->getPosition()), TILESIZE * ROOMSIZE)]);
            }
        }
    }
}
void Game::changeToRoom(Room* newRoom) {
    // Als de huidige kamer al de kamer is waarin de speler zich bevindt, doe niets
    if (currentRoom == newRoom) {
        return;
    }

    // Anders verzet hem naar de nieuwe kamer
    newRoom->playerpointer = currentRoom->playerpointer; // Maak de playerpointer van de nieuwe kamer, de playerpointer van de huidige kamer
    currentRoom->removeEntity(currentRoom->playerpointer); // Haal de speler uit de oude kamer
    currentRoom = newRoom; // Verhuis de currentRoom naar de room die nu op het scherm getoond moet worden
    currentRoom->addEntity(currentRoom->playerpointer); // En voeg de speler toe aan de nieuwe kamer
}

