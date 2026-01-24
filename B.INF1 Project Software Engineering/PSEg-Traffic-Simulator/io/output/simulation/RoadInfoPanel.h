//============================================================================
// Name        : RoadInfoPanel.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class for the road info panel
//============================================================================
#ifndef ROAD_INFO_PANEL_H
#define ROAD_INFO_PANEL_H

#include <SFML/Graphics.hpp>
#include "../../../classes/Road.h"

/**
 * @brief Class for the road info panel
 */
class RoadInfoPanel {
public:
    /**
     * @brief Default constructor
     *
     * \n ENSURE(this->properlyInitialized(),
     *           "Error: Constructor of 'RoadInfoPanel' must end in a correctly initialized state.")
     */
    RoadInfoPanel();

    /**
     * @brief Sets the font used for labels & buttons
     *
     * \n REQUIRE(this->properlyInitialized(),
     *           "Error: RoadInfoPanel's constructor did not initialize properly")
     * \n REQUIRE(f.getInfo().family != "",
     *           "Error: 'font' cannot be NULL or empty.")
     * \n ENSURE(font == &f,
     *           "Error: font was not updated correctly")
     */
    void setFont(sf::Font& f);

    /**
     * @brief Selects the road for which information is shown
     *
     * \n REQUIRE(this->properlyInitialized(),
     *           "Error: RoadInfoPanel's constructor did not initialize properly")
     * \n REQUIRE(road == nullptr || road->properlyInitialized(),
     *           "Error: Road's constructor did not initialize properly")
     * \n ENSURE(selectedRoad == road,
     *           "Error: selectedRoad was not updated correctly")
     */
    void setRoad(Road* road);

    /**
     * @brief Draws the panel
     *
     * \n REQUIRE(this->properlyInitialized(),
     *           "Error: RoadInfoPanel's constructor did not initialize properly")
     * \n REQUIRE(font != nullptr,
     *           "Error: 'font' cannot be NULL or empty.")
     */
    void draw(sf::RenderWindow& window, sf::Vector2u windowSize);

    /**
     * @brief Checks whether a road starts at an intersection
     *
     * \n REQUIRE(road != nullptr,
     *           "Error: 'road' cannot be NULL or empty.")
     * \n REQUIRE(road->properlyInitialized(),
     *           "Error: Road's constructor did not initialize properly")
     */
    bool roadStartsAtIntersection(Road* road);

    // ---------- Buttons (exposed for event handling) -------------------
    sf::RectangleShape& getSetGeneratorButton();
    sf::RectangleShape& getVehicleTypeButton();
    sf::RectangleShape& getVehicleSpawnButton();

    /**
     * @brief Checks for correct initialization
     */
    [[nodiscard]] bool properlyInitialized() const;

private:
    RoadInfoPanel* _initCheck = nullptr;

    Road* selectedRoad = nullptr;
    sf::Font* font = nullptr;

    sf::RectangleShape panelBackground;

    sf::RectangleShape vehicleTypeButton;
    sf::Text vehicleTypeLabel;

    sf::RectangleShape vehicleSpawnButton;
    sf::Text vehicleSpawnLabel;

    sf::RectangleShape setGeneratorButton;
    sf::Text setGeneratorLabel;

    // --- Helper method to draw label-value pairs ---------------------------
    void drawLabelPair(sf::RenderWindow& window, const std::string& label, const std::string& value, float& yOffset, float xPadding);
};
#endif
