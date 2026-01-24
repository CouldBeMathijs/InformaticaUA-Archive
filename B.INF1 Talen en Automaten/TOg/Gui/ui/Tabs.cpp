#include "Tabs.h"
#include <SFML/Graphics/RenderTarget.hpp>

Tabs::Tabs(const sf::Font& font, const std::vector<std::string>& names) {
    for (auto& n : names) {
        triggers.push_back(std::make_shared<Button>(n, font));
    }
}

void Tabs::setPosition(sf::Vector2f pos) {
    float x = pos.x;
    for (auto& btn : triggers) {
        btn->setSize({150, 30});
        btn->setPosition({x, pos.y});
        x += 160;
    }
}

void Tabs::setSize(sf::Vector2f /*size*/) {
    // vaste grootte voor demo
}

void Tabs::draw(sf::RenderTarget& target) {
    for (size_t i = 0; i < triggers.size(); ++i) {
        // highlight active
        if (int(i) == activeIndex) {
            triggers[i]->getBox().setOutlineColor(sf::Color::Yellow);
        } else {
            triggers[i]->getBox().setOutlineColor(sf::Color::White);
        }
        triggers[i]->draw(target);
    }
}