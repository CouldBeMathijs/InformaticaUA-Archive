//============================================================================
// Name        : Button.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : General SFML button class
//============================================================================
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

/**
 * @brief General SFML Button class
 */
class Button {
public:
    /**
     * @brief SFML buttons
     * @param size The size of the button
     * @param position The position of the button on the screen
     * @param label The label to be shown on the button
     * @param font The font the button label is displayed in
     *
     * \n REQUIRE(size.x > 0 && size.y > 0, "Error: Button size cannot be negative.")
     * \n REQUIRE(!label.empty(), "Error: 'label' cannot be NULL or empty.")
     * \n REQUIRE(font is not empty, "Error: 'font' cannot be NULL or empty.")
     * \n ENSURE(text.getString() == label, "Error: Calculated value of 'label' does not match said value in text.getString()")
     */
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& label, sf::Font& font);

    // ───────────────────────────────────────────────────────────
    // Copy-/move-semantics – to make sure _initCheck is always right
    // ───────────────────────────────────────────────────────────
    Button(const Button&  other)            noexcept;
    Button& operator=(const Button& other)  noexcept;

    Button(Button&&       other)            noexcept;
    Button& operator=(Button&&      other)  noexcept;

    /**
     * @brief Draws the windows
     * \n REQUIRE(this->properlyInitialized(), properInitializationRequire("Button"))
     * @param window The window to draw
     */
    void draw(sf::RenderWindow& window) const;

    /**
     * @brief Checks whether the mouse is clicked
     * \n REQUIRE(this->properlyInitialized(), "Error: Button's constructor did not initialize properly")
     * @param mousePos The position of the mouse
     */
    bool isClicked(const sf::Vector2f& mousePos) const;

    /**
     * @brief Changes the hover status of the button
     * \n REQUIRE(this->properlyInitialized(), "Error: Button's constructor did not initialize properly")
     * @param hovered The hover status of the button
     */
    void setHovered(bool hovered);

    /**
     * @brief Shows whether the button is correctly initialized
     * @return true when properly initialized
     */
    bool properlyInitialized() const;

private:
    sf::RectangleShape shape;
    sf::Text text;
    bool hovered = false;

    Button* _initCheck = nullptr;
};
#endif