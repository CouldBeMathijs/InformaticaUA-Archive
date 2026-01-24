#pragma once
#include "UIElement.h"
#include <functional>

class Button : public UIElement {
public:
    Button(std::string label, const sf::Font& font);

    void setPosition(sf::Vector2f pos) override;
    void setSize(sf::Vector2f size) override;
    void draw(sf::RenderTarget& target) override;

    void setOnClick(std::function<void()> cb) { onClick = std::move(cb); }

    void handleEvent(const sf::Event& evt) override;

    sf::RectangleShape getBox() { return box; };
    std::function<void()> onClick;
private:
    sf::RectangleShape box;
    sf::Text text;

};