#include "ButtonManager.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"

void ButtonManager::addButton(const sf::Vector2f& size, const sf::Vector2f& position,
                               const std::string& label, sf::Font& font, ButtonCallback callback) {
    REQUIRE(size.x > 0 && size.y > 0, ErrorMessagesInstructions::mustBePositive("Button size"));
    REQUIRE(!label.empty(), ErrorMessagesInstructions::missingParameter("label"));
    REQUIRE(font.getInfo().family != "", ErrorMessagesInstructions::missingParameter("font"));

    buttons.emplace_back(size, position, label, font);
    callbacks.emplace_back(std::move(callback));

    REQUIRE(buttons.back().properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));
}

void ButtonManager::handleClick(const sf::Vector2f& mousePos) const {
    for (size_t i = 0; i < buttons.size(); ++i) {
        REQUIRE(buttons[i].properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));
        if (buttons[i].isClicked(mousePos)) {
            if (i < callbacks.size()) {
                REQUIRE(callbacks[i], ErrorMessagesInstructions::missingParameter("callback"));
                callbacks[i]();
            }
            break;
        }
    }
}

void ButtonManager::handleHover(const sf::Vector2f& mousePos) {
    for (auto& btn : buttons) {
        REQUIRE(btn.properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));
        btn.setHovered(btn.isClicked(mousePos));
    }
}

void ButtonManager::draw(sf::RenderWindow& window) const {
    for (const auto& btn : buttons) {
        REQUIRE(btn.properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));
        btn.draw(window);
    }
}