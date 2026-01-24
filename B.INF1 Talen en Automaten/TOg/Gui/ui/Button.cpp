#include "Button.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

Button::Button(std::string label, const sf::Font& font) {
    box.setFillColor({50,50,60});
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2.f);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
}

void Button::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    auto b = box.getGlobalBounds();
    auto t = text.getLocalBounds();
    text.setPosition(
      pos.x + (b.width - t.width)/2 - t.left,
      pos.y + (b.height - t.height)/2 - t.top
    );
}

void Button::setSize(sf::Vector2f size) {
    box.setSize(size);
    setPosition(box.getPosition());
}

void Button::draw(sf::RenderTarget& target) {
    target.draw(box);
    target.draw(text);
}

void Button::handleEvent(const sf::Event& evt) {
    if (evt.type == sf::Event::MouseButtonPressed  && evt.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp{ float(evt.mouseButton.x), float(evt.mouseButton.y) };
        if (box.getGlobalBounds().contains(mp)) {
            if (onClick) onClick();
        }
    }
}