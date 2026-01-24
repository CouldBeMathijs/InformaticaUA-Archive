#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <memory>
#include "Button.h"

class Tabs : public UIElement {
public:
    Tabs(const sf::Font& font, const std::vector<std::string>& names);
    void setPosition(sf::Vector2f pos) override;
    void setSize(sf::Vector2f size) override;
    void draw(sf::RenderTarget& target) override;

    void setOnTabChanged(std::function<void(int)> cb) { onTabChanged = std::move(cb); }

private:
    std::vector<std::shared_ptr<Button>> triggers;
    int activeIndex = 0;

    std::function<void(int)> onTabChanged;
};