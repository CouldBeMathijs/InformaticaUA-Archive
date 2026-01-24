#pragma once
#include <SFML/Graphics.hpp>

class UIElement {
public:
    virtual ~UIElement() = default;
    virtual void setPosition(sf::Vector2f pos) = 0;
    virtual void setSize(sf::Vector2f size) { /* optioneel */ }
    virtual void draw(sf::RenderTarget& target) = 0;

    virtual void handleEvent(const sf::Event& evt) {}
};