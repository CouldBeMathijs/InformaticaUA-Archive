//
// Created by Tim Apers on 29/10/2024.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
//#include "Game.h"
using namespace std;
struct Position {
    unsigned int x;
    unsigned int y;

    Position(unsigned int x, unsigned int y);

    Position();

    bool operator<(const Position& other) const {
        if (x < other.x) return true;
        if (x == other.x && y < other.y) return true;
        return false;
    }
};
class Entity {
public:
    Entity();

    virtual void update(sf::Event* event);

    virtual int getAttackPower() const;

    virtual void setAttackPower(int);

    explicit Entity(const Position &position);

    const Position &getPosition() const;

    void setPosition(const Position &position);

    bool standsOn(const Entity& other) const;

    // Onderstaande functies niet aanpassen!
    void setSprite(const std::string &img_path);
    void render(sf::RenderWindow *painter);
    ~Entity() = default;


protected:
    Position position;
    sf::Sprite sprite;
    sf::Texture texture;

};


class Player : public Entity {
private:
    int attackPower;
public:

    void update(sf::Event* event) override;

    int getAttackPower() const override;

    void setAttackPower(int attackPower) override;

    explicit Player(Position position);

    Player();

};

class Weapon : public Entity {
public:
    explicit Weapon(Position position);

};

class Wall : public Entity {
public:
    explicit Wall(Position position);

};

class Floor : public Entity {
public:
    explicit Floor(Position position);

};

class Enemy : public Entity {
public:
    explicit Enemy(Position position);
};

#endif //ENTITY_H
