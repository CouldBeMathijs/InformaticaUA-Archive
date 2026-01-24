//
// Created by Tim Apers on 29/10/2024.
//

#include "Entity.h"
#include <iostream>

void Entity::setSprite(const std::string& img_path) { //NEBM
    if (!this->texture.loadFromFile(img_path)) {
        std::cerr << "Failed to load texture" << std::endl;
    }
    this->sprite.setTexture(this->texture);
    sf::Vector2u size = this->texture.getSize();
    float scaleX = 100.0f / size.x;
    float scaleY = 100.0f / size.y;
    this->sprite.setScale(scaleX, scaleY);
}

void Entity::render(sf::RenderWindow *window) { //NEDM
    sprite.setPosition(position.x%700, position.y%700);
    window->draw(sprite);
}

void Entity::update(sf::Event* event) { //NEBM

}

Entity::Entity(const Position &position) : position(position) {}

const Position &Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const Position &newPosition) {
    Entity::position = newPosition;
}

bool Entity::standsOn(const Entity& other) const {
    // return true als de positie van de ene en de andere entiteit hetzelfde zijn
    return this->position.x == other.position.x && this->position.y == other.position.y;
}


Entity::Entity() {
    //Als er geen position is meegegeven, plaats hem dan op 0,0
    this->position.x = 0;
    this->position.y = 0;
}

int Entity::getAttackPower() const {
    //Error generated when virtual called in stead of override function
    cerr << "getAttackPower() called on non-player entity" << endl;
    return -1;
}

void Entity::setAttackPower(int) {
    //Error generated when virtual called in stead of override function
    cerr << "setAttackPower() called on non-player entity" << endl;
}


void Player::update(sf::Event* event) {
    switch (event->key.code) {
        case sf::Keyboard::Left:
            position.x -= 100;
            break;
        case sf::Keyboard::Right:
            position.x += 100;
            break;
        case sf::Keyboard::Up:
            position.y -= 100;
            break;
        case sf::Keyboard::Down:
            position.y += 100;
            break;
        default: break;
    }
}

int Player::getAttackPower() const {
    return attackPower;
}

void Player::setAttackPower(int attackPower) {
    Player::attackPower = attackPower;
}
Player::Player(Position position) : Entity(position){
    attackPower = 0;
}

Player::Player() {
attackPower = 0;
}

Weapon::Weapon(Position position) : Entity(position){}

Wall::Wall(Position position) : Entity(position){}

Enemy::Enemy(Position position) : Entity(position){}

Floor::Floor(Position position) : Entity(position){}

Position::Position(unsigned int x, unsigned int y) : x(x), y(y) {}

Position::Position() {
    this->x = 0;
    this->y = 0;
}
