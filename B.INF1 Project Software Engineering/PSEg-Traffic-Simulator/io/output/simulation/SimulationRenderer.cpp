#include "SimulationRenderer.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"
#include "../../../classes/Stop.h"
#include "../LayoutUtils.h"
#include "ResourceManager.h"
#include <cmath>
#include <iostream>

RoadRenderer::RoadRenderer(ResourceManager& textureManager)
        : textureManager(textureManager) {}

void RoadRenderer::renderRoad(sf::RenderWindow& window, Road* road, const sf::Vector2f& origin, float angleDeg, const sf::Vector2f& offset, float pixelsPerMeter) {
    REQUIRE(road != nullptr,  ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

    sf::Texture* texture = textureManager.getTexture("road");
    if (!texture) return;

    float angleRad = angleDeg * 3.14159f / 180.f;
    sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));
    float scale = pixelsPerMeter / static_cast<float>(texture->getSize().y);

    for (size_t i = 0; i < road->getLength(); ++i) {
        sf::Sprite sprite(*texture);
        sprite.setOrigin(texture->getSize().x / 2.f, 0.f);
        sprite.setScale(scale, scale);
        sprite.setRotation(angleDeg + 90.f);
        sprite.setPosition(origin + direction * (i * pixelsPerMeter) + offset);
        window.draw(sprite);
    }
}

void RoadRenderer::renderIntersection(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& offset, float scale) {
    sf::Texture* texture = textureManager.getTexture("intersection");
    if (!texture) return;

    sf::Sprite sprite(*texture);
    sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
    sprite.setScale(scale, scale);
    sprite.setPosition(position + offset);
    window.draw(sprite);
}

void RoadRenderer::renderBusStop(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& offset, float scale) {
    sf::Texture* texture = textureManager.getTexture("bus_stop");
    if (!texture) {
        // fallback naar een cirkel als texture ontbreekt
        sf::CircleShape shape(5.f * scale);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(position + offset);
        window.draw(shape);
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
    sprite.setScale(scale, scale);
    sprite.setPosition(position + offset);
    window.draw(sprite);
}

void RoadRenderer::renderTrafficLight(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& offset, float scale, LightState state, bool isVisible) {
    //if (!isVisible) return;
    sf::Texture* texture = textureManager.getTexture("traffic_light");
    if (!texture) {
        sf::CircleShape shape(4.f * scale);
        switch (state) {
            case LightState::GREEN:
                if (!isVisible) return;
                shape.setFillColor(sf::Color::Green);
            break;
            case LightState::RED:
                if (!isVisible) shape.setFillColor(sf::Color(100,100,100));
                else shape.setFillColor(sf::Color::Red);
            break;
            case LightState::ORANGE:
                if (!isVisible) return;
                shape.setFillColor(sf::Color(255, 140, 0));
            break;
        }
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(position + offset);
        window.draw(shape);
        return;
    }

    sf::Sprite sprite(*texture);
    sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
    sprite.setScale(scale, scale);
    sprite.setPosition(position + offset);
    window.draw(sprite);
}

VehicleRenderer::VehicleRenderer(ResourceManager& textureManager)
        : textureManager(textureManager) {}

void VehicleRenderer::renderVehicle(sf::RenderWindow& window, Vehicle* vehicle, const sf::Vector2f& origin, const sf::Vector2f& direction, const sf::Vector2f& offset, float pixelsPerMeter) {
    REQUIRE(vehicle != nullptr, ErrorMessagesInstructions::missingParameter("vehicle"));
    REQUIRE(vehicle->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Vehicle"));

    const std::string& texPath = vehicle->getTexture();
    sf::Texture* texture = textureManager.getTexture(texPath);

    if (!texture) {
        // Try to load it once
        if (!textureManager.loadTexture(texPath, texPath)) {
            std::cerr << "Failed to load texture for vehicle: " << texPath << std::endl;
            return;
        }
        texture = textureManager.getTexture(texPath);
        if (!texture) return;
    }

    sf::Sprite sprite(*texture);
    sprite.setOrigin(texture->getSize().x / 2.f, 0.f);

    float targetLength = vehicle->getLength() * pixelsPerMeter;
    float scale = targetLength / static_cast<float>(texture->getSize().y);
    sprite.setScale(scale, scale);

    sprite.setRotation(std::atan2(direction.y, direction.x) * 180.f / 3.14159f + 90.f);
    sprite.setPosition(origin + direction * static_cast<float>(vehicle->getPosition() * pixelsPerMeter) + offset);

    window.draw(sprite);
}


SimulationRenderer::SimulationRenderer(ResourceManager& textureManager)
        : textureManager(textureManager), roadRenderer(textureManager), vehicleRenderer(textureManager) {}

void SimulationRenderer::render(sf::RenderWindow& window, Simulation* simulation, const sf::Vector2f& offset, float pixelsPerMeter) {
    REQUIRE(simulation != nullptr, ErrorMessagesInstructions::missingParameter("simulation"));
    REQUIRE(simulation->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Simulation"));

    using namespace Utils;

    auto components = groupConnectedRoads(simulation->getRoads());
    if (components.empty()) return;

    computeLayoutForChain(components[0], roadStartPositions, roadAngles);

    // Wegen tekenen
    for (Road* road : components[0]) {
        sf::Vector2f origin = roadStartPositions[road] * pixelsPerMeter;
        float angleDeg = roadAngles[road];
        roadRenderer.renderRoad(window, road, origin, angleDeg, offset, pixelsPerMeter);
    }

    // Intersections verzamelen
    std::unordered_map<Intersection*, sf::Vector2f> intersectionPositions;
    for (Road* road : components[0]) {
        sf::Vector2f origin = roadStartPositions[road] * pixelsPerMeter;
        float angleRad = roadAngles[road] * 3.14159f / 180.f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        for (const auto& binding : road->getIntersectionBindings()) {
            if (!binding.intersection) continue;
            sf::Vector2f pos = origin + direction * static_cast<float>(binding.position * pixelsPerMeter);
            intersectionPositions[binding.intersection] = pos;
        }
    }

    // Intersections tekenen
    for (const auto& [intersection, pos] : intersectionPositions) {
        sf::Texture* roadTexture = textureManager.getTexture("road");
        sf::Texture* intersectionTexture = textureManager.getTexture("intersection");

        if (roadTexture && intersectionTexture) {
            float roadScale = pixelsPerMeter / static_cast<float>(roadTexture->getSize().y);
            float intersectionSize = static_cast<float>(roadTexture->getSize().x) * roadScale;

            float scale = intersectionSize / static_cast<float>(intersectionTexture->getSize().x);

            roadRenderer.renderIntersection(window, pos, offset, scale);
        }
    }

    // Stops tekenen
    for (Road* road : components[0]) {
        sf::Vector2f origin = roadStartPositions[road] * pixelsPerMeter;
        float angleRad = roadAngles[road] * 3.14159f / 180.f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        for (Stop* stop : road->getStops()) {
            sf::Vector2f pos = origin + direction * static_cast<float>(stop->getPosition() * pixelsPerMeter);

            if (dynamic_cast<BusStop*>(stop)) {
                roadRenderer.renderBusStop(window, pos, offset, pixelsPerMeter / 10.f);
            } else if (auto* trafficLight = dynamic_cast<TrafficLight*>(stop)) {
                if (auto* smartLight = dynamic_cast<SmartTrafficLight*>(trafficLight)) {
                    roadRenderer.renderTrafficLight(window, pos, offset, pixelsPerMeter / 10.f, smartLight->getCurrentState(), smartLight->isVisible());
                } else {
                    roadRenderer.renderTrafficLight(window, pos, offset, pixelsPerMeter / 10.f, trafficLight->getCurrentState());
                }
            }
        }
    }

    // Voertuigen tekenen
    for (Road* road : components[0]) {
        sf::Vector2f origin = roadStartPositions[road] * pixelsPerMeter;
        float angleRad = roadAngles[road] * 3.14159f / 180.f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        for (Vehicle* vehicle : road->getVehicles()) {
            vehicleRenderer.renderVehicle(window, vehicle, origin, direction, offset, pixelsPerMeter);
        }
    }
}

std::unordered_map<Road *, sf::Vector2f> SimulationRenderer::getRoadStartPositions() {
    return roadStartPositions;
}

std::unordered_map<Road *, float> SimulationRenderer::getRoadAngles() {
    return roadAngles;
}
