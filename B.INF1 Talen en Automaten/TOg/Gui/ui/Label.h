#pragma once
#include "UIElement.h"

class Label : public UIElement {
public:
    Label(std::string txt, const sf::Font& font, unsigned int charSize);
    void setPosition(sf::Vector2f pos) override;
    void draw(sf::RenderTarget& target) override;
    void setText(const std::string& txt);

private:
    sf::Text text;
};