//
// Created by Tim Apers on 29/10/2024.
//

#include <iostream>
#include "Room.h"
void Room::render(sf::RenderWindow* window) { //NEBM
    Player* player = nullptr;
    std::vector<Entity*> others = {};
    for (Entity* entity : entities) {
        if (auto* floor = dynamic_cast<Floor*>(entity)) {floor->render(window);}
        else if (auto* p = dynamic_cast<Player*>(entity)) { player = p;}
        else { others.push_back(entity); }
    }
    for (Entity* e : others) { e->render(window); }
    if (player) {
        player->render(window);
        this->playerpointer = player;
    }

}

void Room::update(sf::Event* event) {
    Position oldposition; //Maak een position aan (standaard 0,0)
    bool havebeenhere = false;
    for (Entity* entity : entities) { // Voor alle entiteiten in deze kamer
        if (dynamic_cast<Player*>(entity)){ // Als deze entiteit een speler is
            if (!havebeenhere) {
                oldposition = entity->getPosition(); // Sla er dan de positie van op
                playerpointer->update(event);
                havebeenhere = true;
            }
        } else {
            entity->update(event);
        }
    }
    for (Entity* entity : entities) { // Voor alle enteiten in deze kamer
        if (entity->standsOn(*playerpointer)){ // Als deze entiteit op dezelfde plaats staat als de speler
            if (dynamic_cast<Wall*>(entity)){ // En het een muur is
                playerpointer->setPosition(oldposition); // Verplaats de speler dan terug
                continue;
            }
            if (dynamic_cast<Weapon*>(entity)) { // Als de speler op een wapen staat
                removeEntity(entity); // Verwijder het wapen uit de kamer
                delete entity; // Verwijder de entiteit van het wapen uit het spel
                playerpointer->setAttackPower(playerpointer->getAttackPower() + 1); // Voeg 1 toe bij de attackpower
                continue;
            }
            if (dynamic_cast<Enemy*>(entity)) { // Als de speler op een vijand staat
                if (playerpointer->getAttackPower() > 0) { // En de AttackPower > 0
                    removeEntity(entity); // Verwijder dan de vijand uit de kamer
                    playerpointer->setAttackPower(playerpointer->getAttackPower() - 1);
                    delete entity; // En uit het geheugen
                    continue;
                } else {
                    playerpointer->setPosition(oldposition); // Anders, doe alsof het een muur is
                    continue;
                }

            }
        }
    }
}

const vector<Entity *> &Room::getEntities() const {
    return entities;
}

void Room::addEntity(Entity* newEntity) {
    entities.push_back(newEntity);
}
void Room::addEntity(Player* newEntity) {
    entities.push_back(newEntity);
}

void Room::removeEntity(Entity* entity) {
    for (uint i = 0; i < entities.size(); ++i) { // Doorloop alle entities in de lijst
        if (entities[i] == entity) { // Als dat de te verwijderen entiteit is
            entities.erase(entities.begin() + i); // Verwijder die dan
            break; // En stop met verder zoeken
        }
    }
}




Room::Room() {
    playerpointer = nullptr;
}

