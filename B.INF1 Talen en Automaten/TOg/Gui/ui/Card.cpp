#include "Card.h"
#include <SFML/Graphics/RenderTarget.hpp>

Card::Card(const sf::Font& f, std::string t, std::string d)
: font(f)
{
    background.setFillColor({40,40,50});
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2.f);

    title.setFont(font);
    title.setString(t);
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);

    description.setFont(font);
    description.setString(d);
    description.setCharacterSize(14);
    description.setFillColor({200,200,200});
}

void Card::setPosition(sf::Vector2f pos) {
    background.setPosition(pos);
    title.setPosition(pos + sf::Vector2f{20,10});
    description.setPosition(pos + sf::Vector2f{20,35});
    // children krijgen eigen pos
}

void Card::setSize(sf::Vector2f size) {
    background.setSize(size);
}

void Card::addChild(std::shared_ptr<UIElement> child) {
    children.push_back(child);
}

void Card::draw(sf::RenderTarget& target) {
    target.draw(background);
    target.draw(title);
    target.draw(description);
    for (auto& c : children)
        c->draw(target);
}

void Card::handleEvent(const sf::Event& evt) {
    for (auto& child : children) {
        child->handleEvent(evt);
    }
}

std::string Card::getTitle() const {
    return title.getString();
}

const std::vector<std::shared_ptr<UIElement>>& Card::getChildren() const {
    return children;
}

sf::Vector2f Card::getPosition() const {
    return background.getPosition();
}

sf::Vector2f Card::getSize() const {
    return background.getSize();
}