#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "ui/UIElement.h"

class MainGui {
public:
    MainGui(sf::RenderWindow& window, const sf::Font& font);
    void draw();

    void handleEvent(const sf::Event& evt);

    void refresh();

    void updateSupportedLanguages();

    void updateSpriteScale();

    void openSvgInBrowser(const std::string &svgPath);

private:
    sf::RenderWindow& window;
    sf::Font const& font;
    std::vector<std::shared_ptr<UIElement>> elements;

    sf::Texture languageTexture;
    sf::Sprite languageSprite;
    std::string currentSvgPath;
    sf::Sprite outputSprite;

    float zoomLevel = 1.0f;
    const float zoomStep = 0.1f;
    float baseScale = 1.0f;
};