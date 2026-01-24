//============================================================================
// Name        : ButtonManager.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class to manage all individual SFML buttons
//============================================================================
#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Button.h"

/**
 * @brief Class to manage all individual SFML buttons
 */
class ButtonManager {
public:
    using ButtonCallback = std::function<void()>;

    /**
     * @brief Add a new button to the manager
     * @param size Dimensions of the button
     * @param position Position on the screen
     * @param label The text on the button
     * @param font The font for the text
     * @param callback The action executed when the button is clicked
     *
     * \n REQUIRE(size.x > 0 && size.y > 0, "Error: Button size cannot be negative.")
     * \n REQUIRE(!label.empty(), "Error: 'label' cannot be NULL or empty.")
     * \n REQUIRE(font is not empty, "Error: 'font' cannot be NULL or empty.")
     * \n REQUIRE(button is properlyInitialized, "Error: Button's constructor did not initialize properly")
     */
    void addButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label, sf::Font& font, ButtonCallback callback);

    /**
     * @brief Handle a click on the screen
     * \n REQUIRE(each button is properlyInitialized, "Error: Button's constructor did not initialize properly")
     * \n REQUIRE(callback != nullptr, "Error: 'callback' cannot be NULL or empty.")
     */
    void handleClick(const sf::Vector2f& mousePos) const;

    /**
     * @brief Handle hover state based on mouse position
     * \n REQUIRE(each button is properlyInitialized, "Error: Button's constructor did not initialize properly")
     */
    void handleHover(const sf::Vector2f& mousePos);

    /**
     * @brief Draw all buttons
     * \n REQUIRE(each button is properlyInitialized, "Error: Button's constructor did not initialize properly")
     */
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<Button> buttons;
    std::vector<ButtonCallback> callbacks;
};
#endif
