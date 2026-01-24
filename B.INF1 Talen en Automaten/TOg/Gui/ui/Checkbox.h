#pragma once
#include "UIElement.h"
#include <functional>

class Checkbox : public UIElement {
public:
    Checkbox(const sf::Font& font);

    void setPosition(sf::Vector2f pos) override;
    void setSize(sf::Vector2f size) override;
    void draw(sf::RenderTarget& target) override;
    void handleEvent(const sf::Event& evt) override;

    void setLabel(const std::string& text);

    void setChecked(bool val);

    void setOnToggle(std::function<void(bool)> cb) { onToggle = std::move(cb); }

    bool isChecked() const { return checked; }

private:
    sf::RectangleShape box;
    sf::Text label;
    bool checked = false;
    std::function<void(bool)> onToggle;
};