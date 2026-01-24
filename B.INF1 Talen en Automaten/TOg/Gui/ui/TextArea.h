#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

#include "UIElement.h"

class TextArea : public UIElement{
public:
    TextArea(const std::string& placeholder, const sf::Font& font);

    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);
    void draw(sf::RenderTarget& target);
    void handleEvent(const sf::Event& evt);

    const std::string& getText() const { return content; }
    void setOnTextChanged(std::function<void(const std::string&)> callback) {
        onTextChanged = std::move(callback);
    }

private:
    std::string wrapText(const std::string& str, const sf::Text& prototype, float maxWidth);
    void updateDisplayText();

    sf::RectangleShape box;
    sf::Text placeholder;
    sf::Text contentText;

    std::string content;
    std::function<void(const std::string&)> onTextChanged;

    bool hasFocus = false;
    bool selectedAll = false;

    int cursorIndex = 0;
    int selectionStart = -1;
    int selectionEnd = -1;

    float scrollOffsetY = 0.f;
};
