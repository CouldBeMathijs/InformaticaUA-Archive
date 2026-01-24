#include "TextInput.h"
#include <SFML/Graphics/RenderTarget.hpp>

TextInput::TextInput(std::string ph, const sf::Font& font) {
    box.setFillColor({30,30,30});
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);
    placeholder.setFont(font);
    placeholder.setString(ph);
    placeholder.setCharacterSize(16);
    placeholder.setFillColor({150,150,150});
}

void TextInput::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    placeholder.setPosition(pos + sf::Vector2f{5,5});
}

void TextInput::setSize(sf::Vector2f size) {
    box.setSize(size);
}

void TextInput::draw(sf::RenderTarget& target) {
    target.draw(box);
    target.draw(placeholder);
}