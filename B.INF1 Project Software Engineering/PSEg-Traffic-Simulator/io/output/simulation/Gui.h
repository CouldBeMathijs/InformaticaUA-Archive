//============================================================================
// Name        : GUI.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Main SFML Manager class
//============================================================================
#ifndef GUI_H
#define GUI_H

#include "../../../classes/Simulation.h"
#include "ButtonManager.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "RoadInfoPanel.h"
#include "SimulationRenderer.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Struct to keep the road lay out
 */
struct RoadLayout {
    sf::Vector2f origin;
    sf::Vector2f direction;
    float angleDeg{};
    float widthPixels{};
};

/**
 * @brief Struct to keep GUI related settings
 */
struct GuiSettings {
    static constexpr float ClickDragThreshold = 5.f;
    static constexpr float PanelWidth = 300.f;
    static constexpr float PanelY = 100.f;
    static constexpr float PanelBottomMargin = 30.f;
};

/**
 * @brief General SFML manager
 */
class Gui {
public:
    /**
     * @brief Creates a new GUI object for the given simulation.
     *
     * \n REQUIRE(simulation != nullptr, "Error: 'simulation' cannot be NULL or empty.")
     * \n REQUIRE(simulation->properlyInitialized(), "Error: Simulation's constructor did not initialize properly")
     */
    explicit Gui(Simulation* simulation);

    /**
     * @brief Starts the main loop of the GUI.
     *
     * \n REQUIRE(window.isOpen(), "Error: GUI window is not correctly initialized")
     */
    void run();

private:
    /** \brief Loads fonts & textures.
     *
     * \n ENSURE(font.getInfo().family != "", "Error: 'font' cannot be NULL or empty.")
     */
    void loadResources();

    /// Processes all incoming events (mouse, keyboard, etc.).
    void processEvents();

    /** \brief Updates simulation and GUI state.
     *
     * \n REQUIRE(simulation->properlyInitialized(), "Error: Simulation's constructor did not initialize properly")
     */
    void update();

    /// Draws everything on the screen.
    void render();

    /// Called every time the mouse is moved.
    void handleMouseMoved();

    /**
     * @brief Zooms in/out when the mouse wheel is scrolled.
     *
     * \n REQUIRE(scrollEvent.wheel == sf::Mouse::VerticalWheel || scrollEvent.wheel == sf::Mouse::HorizontalWheel, "Error: scroll event was not correctly registered")
     */
    void handleMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& scrollEvent);

    /**
     * @brief Processes a click in world coordinates.
     *
     * \n REQUIRE(worldPos.x >= 0 && worldPos.y >= 0, "Error: 'worldPos' is out of bounds")
     */
    void handleClickAt(const sf::Vector2f& worldPos);

    /// Checks whether a road was clicked.
    void checkClickedOnRoad(const sf::Vector2f& worldPos);

    /// Checks whether a traffic light was clicked.
    void checkClickedOnTrafficLight(const sf::Vector2f& worldClick);

    /**
     * @brief Returns whether the given road starts at an intersection.
     *
     * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.")
     * \n REQUIRE(road->properlyInitialized(), "Error: Road's constructor did not initialize properly")
     */
    bool roadStartsAtIntersection(Road* road);

    sf::RenderWindow window;
    sf::Font font;

    Simulation* simulation;
    ResourceManager resourceManager;
    SimulationRenderer simulationRenderer;
    RoadInfoPanel roadInfoPanel;
    ButtonManager buttonManager;
    Camera camera;

    std::unordered_map<Road*, RoadLayout> roadLayouts;

    sf::Clock tickClock;
    sf::Time tickInterval;
    bool paused = false;
    int speedMultiplier = 1;

    bool dragging = false;
    sf::Vector2f dragStartMouse;
    sf::Vector2f dragMouseStartScreen;

    float pixelsPerMeter = 20.f;

    Road* clickedRoad = nullptr;
};
#endif
