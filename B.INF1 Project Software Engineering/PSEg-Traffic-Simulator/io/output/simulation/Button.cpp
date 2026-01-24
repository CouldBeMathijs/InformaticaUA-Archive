#include "Button.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"

Button::Button(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &label, sf::Font &font) {
    REQUIRE(size.x > 0 && size.y > 0, ErrorMessagesInstructions::mustBePositive("Button size"));
    REQUIRE(!label.empty(), ErrorMessagesInstructions::missingParameter("label"));
    REQUIRE(font.getInfo().family != "", ErrorMessagesInstructions::missingParameter("font"));

    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(230,230,230));
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color(180, 180, 180));

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
    text.setFillColor(sf::Color(50, 50, 50));

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    text.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f);

    _initCheck = this;

    ENSURE(text.getString() == label, ErrorMessagesInstructions::mismatchedComputedValue("label", "text.getString"));
}

void Button::draw(sf::RenderWindow& window) const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));

    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f &mousePos) const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));

    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setHovered(bool waver) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Button"));

    this->hovered = waver;
    if (waver) {
        shape.setFillColor(sf::Color(210, 210, 210));
    } else {
        shape.setFillColor(sf::Color(230, 230, 230));
    }
}

bool Button::properlyInitialized() const {
    return _initCheck == this;
}

Button::Button(const Button& other) noexcept            = default;
Button& Button::operator=(const Button& other) noexcept = default;

Button::Button(Button&& other) noexcept { *this = std::move(other); }
Button& Button::operator=(Button&& other) noexcept {
    shape   = std::move(other.shape);
    text    = std::move(other.text);
    hovered = other.hovered;
    _initCheck = this;
    return *this;
}