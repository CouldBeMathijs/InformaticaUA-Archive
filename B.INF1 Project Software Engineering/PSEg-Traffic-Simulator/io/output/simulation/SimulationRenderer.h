//============================================================================
// Name        : SimulationRenderer.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class in charge of rendering the simulation itself
//============================================================================
#ifndef SIMULATION_RENDERER_H
#define SIMULATION_RENDERER_H

#include "../../../classes/Simulation.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * @brief Class in charge of rendering the simulation itself
 */
class RoadRenderer {
public:
    /**
     * @brief Constructor
     * @param textureManager Shared ResourceManager
     */
    explicit RoadRenderer(ResourceManager& textureManager);

    /**
     * @brief Draws a single road segment
     *
     * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.")
     * \n REQUIRE(road->properlyInitialized(), "Error: Road's constructor did not initialize properly")
     */
    void renderRoad(sf::RenderWindow& window, Road* road,
                    const sf::Vector2f& origin, float angleDeg,
                    const sf::Vector2f& offset, float pixelsPerMeter);

    /// Draws an intersection (either a texture or fallback shape)
    void renderIntersection(sf::RenderWindow& window, const sf::Vector2f& pos,
                            const sf::Vector2f& offset, float scale);

    /// Draws a bus stop (either a texture or fallback shape)
    void renderBusStop(sf::RenderWindow& window, const sf::Vector2f& pos,
                       const sf::Vector2f& offset, float scale);

    /// Draws a traffic light (either a texture or fallback shape)
    void renderTrafficLight(sf::RenderWindow& window, const sf::Vector2f& pos,
                            const sf::Vector2f& offset, float scale,
                            LightState state, bool isVisible = true);

private:
    ResourceManager& textureManager;
};

/**
 * @brief Manages vehicle rendering
 */
class VehicleRenderer {
public:
    explicit VehicleRenderer(ResourceManager& textureManager);

    /**
     * @brief Draws a single vehicle on a road segment
     *
     * \n REQUIRE(vehicle != nullptr, "Error: 'vehicle' cannot be NULL or empty.")
     * \n REQUIRE(vehicle->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly")
     */
    void renderVehicle(sf::RenderWindow& window, Vehicle* vehicle,
                       const sf::Vector2f& roadOrigin,
                       const sf::Vector2f& roadDirection,
                       const sf::Vector2f& offset, float pixelsPerMeter);

private:
    ResourceManager& textureManager;
};

/**
 * @brief Manages the rendering of the simulation
 */
class SimulationRenderer {
public:
    /**
     * @brief Constructor
     * @param textureManager Shared ResourceManager
     */
    explicit SimulationRenderer(ResourceManager& textureManager);

    /**
     * @brief Renders the entire simulation scene
     *
     * \n REQUIRE(simulation != nullptr, "Error: 'simulation' cannot be NULL or empty.")
     * \n REQUIRE(simulation->properlyInitialized(), "Error: Simulation's constructor did not initialize properly")
     */
    void render(sf::RenderWindow& window, Simulation* simulation, const sf::Vector2f& offset, float pixelsPerMeter);

    /// Layout information (used by GUI for click detection)
    std::unordered_map<Road*, sf::Vector2f> getRoadStartPositions();
    std::unordered_map<Road*, float>        getRoadAngles();

private:
    ResourceManager& textureManager;
    RoadRenderer roadRenderer;
    VehicleRenderer vehicleRenderer;

    std::unordered_map<Road*, sf::Vector2f> roadStartPositions;
    std::unordered_map<Road*, float> roadAngles;
};
#endif
