#pragma once
#include "UIElement.h"

class TextInput : public UIElement {
public:
    TextInput(std::string placeholder, const sf::Font& font);
    void setPosition(sf::Vector2f pos) override;
    void setSize(sf::Vector2f size) override;
    void draw(sf::RenderTarget& target) override;

private:
    sf::RectangleShape box;
    sf::Text placeholder;
};