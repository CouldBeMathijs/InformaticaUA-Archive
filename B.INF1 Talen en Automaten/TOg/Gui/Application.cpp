#include "Application.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../Languages.h"

Application::Application() : window(sf::VideoMode::getDesktopMode(), "Talen in Automaten GUI") {

    if (!font.loadFromFile("resources/fonts/Roboto-Regular.ttf")) {
        std::cerr << "Error: could not load font\n";
    }

    gui = std::make_unique<MainGui>(window, font);
}

void Application::run() {
    using namespace std::filesystem;
    if (exists("output.png")) remove("output.png");
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Application::processEvents() {
    sf::Event evt;
    while (window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            window.close();
            if (std::filesystem::exists("output.png")) std::filesystem::remove("output.png");
        }
        gui->handleEvent(evt);
    }
}

void Application::update() {
    // geen logic voor nu
}

void Application::render() {
    window.clear(sf::Color(20,20,20));
    gui->draw();
    window.display();
}

std::vector<std::string> Application::getSupportedLanguages() {
    namespace fs = std::filesystem;
    std::vector<std::string> langs;
    try {
        for (const auto& entry : fs::directory_iterator("languages")) {
            if (entry.path().extension() == ".json") {
                langs.push_back(entry.path().stem().string());
            }
        }
        std::sort(langs.begin(), langs.end());
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error reading languages: " << e.what() << std::endl;
    }
    return langs;
}

void Application::saveUserRegexByName(const std::string& name, const std::string& regex) {
    namespace fs = std::filesystem;
    try {
        fs::create_directories("scripts");
        std::string basePath = "scripts/" + name + ".script";
        std::string filePath = basePath;
        int suffix = 1;
        while (fs::exists(filePath)) {
            filePath = "scripts/" + name + ".script" + std::to_string(suffix++);
        }
        std::ofstream file(filePath);
        if (!file) {
            std::cerr << "Unable to create script file for: " << name << std::endl;
            return;
        }
        file << "#regex\n ";
        file << regex;
        file << " \n(^+\?)";
        file.close();
        std::cout << "Saved regex to " << filePath << std::endl;
        std::string jsonPath = "languages/" + name + ".json";
        if (fs::exists(jsonPath)) {
            std::cout << "Removing old JSON file: " << jsonPath << std::endl;
            fs::remove(jsonPath);
        }
        Languages::generateAll();
    } catch (const std::exception& e) {
        std::cerr << "Failed to save regex: " << e.what() << std::endl;
    }
}
