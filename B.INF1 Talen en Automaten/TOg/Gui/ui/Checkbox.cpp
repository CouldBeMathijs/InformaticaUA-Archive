#include "Checkbox.h"
#include <SFML/Graphics/RenderTarget.hpp>

Checkbox::Checkbox(const sf::Font& font) {
    box.setFillColor({20,20,20});
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);

    label.setFont(font);
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::White);
}

void Checkbox::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    label.setPosition(pos + sf::Vector2f{30,-4});
}

void Checkbox::setSize(sf::Vector2f size) {
    box.setSize(size);
}

void Checkbox::draw(sf::RenderTarget& target) {
    target.draw(box);
    if (checked) {
        // draw a tick or fill
        sf::RectangleShape tick({ box.getSize().x/2, box.getSize().y/2 });
        tick.setFillColor(sf::Color::Green);
        tick.setPosition(box.getPosition() + sf::Vector2f{4,4});
        target.draw(tick);
    }
    target.draw(label);
}

void Checkbox::handleEvent(const sf::Event& evt) {
    if (evt.type == sf::Event::MouseButtonPressed &&
        evt.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f m{ float(evt.mouseButton.x), float(evt.mouseButton.y) };
        if (box.getGlobalBounds().contains(m)) {
            checked = !checked;
            if (onToggle) onToggle(checked);
        }
    }
}

void Checkbox::setLabel(const std::string& txt) {
    label.setString(txt);
}

void Checkbox::setChecked(bool val) {
    checked = val;
}