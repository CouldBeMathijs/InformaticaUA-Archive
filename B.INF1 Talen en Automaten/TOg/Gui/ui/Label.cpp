#include "Label.h"
#include <SFML/Graphics/RenderTarget.hpp>

Label::Label(std::string txt, const sf::Font& font, unsigned int cs) {
    text.setFont(font);
    text.setString(txt);
    text.setCharacterSize(cs);
    text.setFillColor(sf::Color::White);
}

void Label::setPosition(sf::Vector2f pos) {
    text.setPosition(pos);
}

void Label::draw(sf::RenderTarget& target) {
    target.draw(text);
}

void Label::setText(const std::string& txt) {
    text.setString(txt);
}