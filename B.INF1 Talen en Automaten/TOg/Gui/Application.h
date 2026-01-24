#pragma once
#include <SFML/Graphics.hpp>
#include "MainGui.h"

class Application {
public:
    Application();
    ~Application() = default;
    void run();

    // Utils
    static std::vector<std::string> getSupportedLanguages();
    static void saveUserRegexByName(const std::string &name,
                                    const std::string &regex);

private:
    sf::RenderWindow window;
    sf::Font font;
    std::unique_ptr<MainGui> gui;

    void processEvents();
    void update();
    void render();
};