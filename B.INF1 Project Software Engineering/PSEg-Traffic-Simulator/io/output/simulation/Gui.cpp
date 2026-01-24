#include "Gui.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"
#include "../3d_rendering/GraphicsExporter.h"
#include "../LayoutUtils.h"
#include "../SimulatieExporter.h"
#include "SimulationRenderer.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr float CLICK_DRAG_THRESHOLD = 5.f;

Gui::Gui(Simulation *simulation) : window(sf::VideoMode::getDesktopMode(), "Traffic Simulator"), simulation(simulation), simulationRenderer(resourceManager) {
    REQUIRE(simulation != nullptr, ErrorMessagesInstructions::missingParameter("simulation"));
    REQUIRE(simulation->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));

    tickInterval = sf::seconds(simulation->getTimePerTick());
    window.setFramerateLimit(60);

    loadResources();

    REQUIRE(font.getInfo().family != "", ErrorMessagesInstructions::missingParameter("font"));

    roadInfoPanel.setFont(font);

    sf::Vector2u winSize = window.getSize();

    // Setup buttons
    buttonManager.addButton({150.f, 50.f}, {10.f, 10.f}, "Play", font, [this]() { paused = false; });
    buttonManager.addButton({150.f, 50.f}, {170.f, 10.f}, "Pause", font, [this]() { paused = true; });
    buttonManager.addButton({50.f, 50.f}, {330.f, 10.f}, "1x", font, [this]() { speedMultiplier = 1; });
    buttonManager.addButton({50.f, 50.f}, {390.f, 10.f}, "2x", font, [this]() { speedMultiplier = 2; });
    buttonManager.addButton({50.f, 50.f}, {450.f, 10.f}, "4x", font, [this]() { speedMultiplier = 4; });
    buttonManager.addButton({50.f, 50.f}, {510.f, 10.f}, "100x", font, [this]() { speedMultiplier = 100; });

    buttonManager.addButton({160.f, 40.f}, {static_cast<float>(winSize.x) - 160 - 10.f, 10.f}, "Export", font, [this]() { GraphicsExporter::exportToImage(this->simulation, "time" + std::to_string(this->simulation->getTime()), "Wireframe", 1000, 1000, 1000); });
}

void Gui::loadResources() {
    resourceManager.loadAllTextures();
    resourceManager.loadFont("default", "./resources/fonts/Anton-Regular.ttf");

    sf::Font* fontPtr = resourceManager.getFont("default");
    if (!fontPtr) {
        std::cerr << "Failed to load default font!\n";
        return;
    }
    REQUIRE(fontPtr != nullptr, ErrorMessagesInstructions::missingParameter("font"));

    font = *fontPtr;

    ENSURE(font.getInfo().family != "", ErrorMessagesInstructions::missingParameter("font"));
}

void Gui::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Gui::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    dragStartMouse = sf::Vector2f(sf::Mouse::getPosition(window));
                    dragMouseStartScreen = sf::Vector2f(sf::Mouse::getPosition(window));
                    dragging = true;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    dragging = false;
                    sf::Vector2f releasePos = sf::Vector2f(sf::Mouse::getPosition(window));
                    float distanceMoved = std::hypot(releasePos.x - dragMouseStartScreen.x, releasePos.y - dragMouseStartScreen.y);
                    if (distanceMoved < CLICK_DRAG_THRESHOLD) {
                        sf::Vector2f worldClick = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        handleClickAt(worldClick);
                    }
                }
                break;

            case sf::Event::MouseMoved:
                handleMouseMoved();
                if (dragging) {
                    sf::Vector2f currentMouse = sf::Vector2f(sf::Mouse::getPosition(window));
                    camera.handleDrag(dragStartMouse, currentMouse);
                    dragStartMouse = currentMouse;
                }
                break;

            case sf::Event::MouseWheelScrolled:
                handleMouseWheelScrolled(event.mouseWheelScroll);
                break;

            default:
                break;
        }
    }
}

void Gui::handleClickAt(const sf::Vector2f& worldClick) {
    buttonManager.handleClick(worldClick);

    float panelX = static_cast<float>(window.getSize().x - GuiSettings::PanelWidth);
    float panelY = 100.f;
    float panelW = 300.f;
    float panelH = static_cast<float>(window.getSize().y) - GuiSettings::PanelY - GuiSettings::PanelBottomMargin;

    if (clickedRoad) {
        REQUIRE(clickedRoad->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

        if (roadInfoPanel.getSetGeneratorButton().getGlobalBounds().contains(worldClick)) {
            if (!clickedRoad->hasVehicleGenerator() && !roadStartsAtIntersection(clickedRoad)) {
                VehicleGenerator* generator = new VehicleGenerator(5, clickedRoad, "random");
                clickedRoad->setVehicleGenerator(generator);

                roadInfoPanel.setRoad(clickedRoad);
                return;
            }
        }

        if (clickedRoad && clickedRoad->hasVehicleGenerator() && !roadStartsAtIntersection(clickedRoad)) {
            auto* generator = clickedRoad->getVehicleGenerator();
            REQUIRE(generator != nullptr, ErrorMessagesInstructions::missingParameter("vehicleGenerator"));

            if (roadInfoPanel.getVehicleTypeButton().getGlobalBounds().contains(worldClick)) {
                generator->toggleVehicleType();
                return;
            }

            if (roadInfoPanel.getVehicleSpawnButton().getGlobalBounds().contains(worldClick)) {
                (void)generator->tryGenerate();
                return;
            }
        }

        if (worldClick.x >= panelX && worldClick.x <= panelX + panelW && worldClick.y >= panelY && worldClick.y <= panelY + panelH) {
            return;
        }
    }

    checkClickedOnRoad(worldClick);
    checkClickedOnTrafficLight(worldClick);
}

void Gui::handleMouseMoved() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    buttonManager.handleHover(mousePos);
}

void Gui::handleMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& scrollEvent) {
    sf::Vector2i mousePixelBefore = sf::Mouse::getPosition(window);
    sf::Vector2f worldBeforeZoom = (sf::Vector2f(mousePixelBefore) - camera.getOffset()) / pixelsPerMeter;

    float zoomFactor = (scrollEvent.delta > 0) ? 1.1f : 0.9f;
    pixelsPerMeter *= zoomFactor;
    pixelsPerMeter = std::clamp(pixelsPerMeter, 5.f, 100.f);

    sf::Vector2f newScreenPos = worldBeforeZoom * pixelsPerMeter + camera.getOffset();
    sf::Vector2f mousePixelAfter = sf::Vector2f(mousePixelBefore);

    camera.addOffset(mousePixelAfter - newScreenPos);
}

void Gui::update() {
    REQUIRE(simulation != nullptr, ErrorMessagesInstructions::missingParameter("simulation"));
    REQUIRE(simulation->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));

    if (!paused && tickClock.getElapsedTime() >= tickInterval) {
        simulation->runTicks(speedMultiplier);
        tickClock.restart();
    }
    roadInfoPanel.setRoad(clickedRoad);
}

void Gui::render() {
    window.clear(sf::Color(30, 30, 30));

    // Simulatie tekenen
    simulationRenderer.render(window, simulation, camera.getOffset(), pixelsPerMeter);
    roadInfoPanel.draw(window, window.getSize());

    // Buttons tekenen
    buttonManager.draw(window);

    window.display();
}

void Gui::checkClickedOnRoad(const sf::Vector2f& worldPos) {
    const auto& roadPositions = simulationRenderer.getRoadStartPositions();
    const auto& roadAngles = simulationRenderer.getRoadAngles();
    sf::Texture* roadTexture = resourceManager.getTexture("road");

    if (!roadTexture) return;

    float roadTileWidth = static_cast<float>(roadTexture->getSize().x);
    float roadTileHeight = static_cast<float>(roadTexture->getSize().y);
    float roadWidthPx = roadTileWidth * (pixelsPerMeter / roadTileHeight);

    sf::Vector2f clickWorld = worldPos - camera.getOffset();

    for (const auto& [road, origin] : roadPositions) {
        REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
        REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

        if (!roadAngles.count(road)) continue;

        float angleDeg = roadAngles.at(road);
        float angleRad = angleDeg * 3.14159f / 180.f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        sf::Vector2f originPx = origin * pixelsPerMeter;
        sf::Vector2f rel = clickWorld - originPx;

        float projLength = rel.x * direction.x + rel.y * direction.y;
        float projWidth = std::abs(rel.x * -direction.y + rel.y * direction.x);

        float roadLengthPx = static_cast<float>(road->getLength()) * pixelsPerMeter;

        if (projLength >= 0 && projLength <= roadLengthPx && projWidth < roadWidthPx / 2.f) {
            clickedRoad = road;
            return;
        }
        clickedRoad = nullptr;
    }
}

void Gui::checkClickedOnTrafficLight(const sf::Vector2f& worldClick) {
    const auto& roadPositions = simulationRenderer.getRoadStartPositions();
    const auto& roadAngles = simulationRenderer.getRoadAngles();

    for (Road* road : simulation->getRoads()) {
        REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
        REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

        if (!roadPositions.count(road) || !roadAngles.count(road)) continue;

        sf::Vector2f origin = roadPositions.at(road) * pixelsPerMeter;
        float angleRad = roadAngles.at(road) * 3.14159f / 180.f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        for (Stop* stop : road->getStops()) {
            auto* trafficLight = dynamic_cast<TrafficLight*>(stop);
            if (!trafficLight) continue;
            if (auto* smartLight = dynamic_cast<SmartTrafficLight*>(trafficLight)) {
                if (!smartLight->isVisible()) {
                    continue;
                }
            }

            sf::Vector2f pos = origin + direction * static_cast<float>(trafficLight->getPosition() * pixelsPerMeter);
            sf::Vector2f screenPos = pos + camera.getOffset();

            float radius = 4.f * (pixelsPerMeter / 10.f);

            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

            if (std::hypot(mousePos.x - screenPos.x, mousePos.y - screenPos.y) <= radius) {
                trafficLight->toggle();

                return;
            }
        }
    }
}

bool Gui::roadStartsAtIntersection(Road* road) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

    for (const auto& binding : road->getIntersectionBindings()) {
        if (binding.position == 0) {
            return true;
        }
    }
    return false;
}
