#include "RoadInfoPanel.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"

RoadInfoPanel::RoadInfoPanel() {
    _initCheck = this;

    vehicleTypeButton.setSize({130.f, 30.f});
    vehicleTypeButton.setFillColor(sf::Color(70, 70, 70));

    vehicleSpawnButton.setSize({130.f, 30.f});
    vehicleSpawnButton.setFillColor(sf::Color(70, 70, 70));

    setGeneratorButton.setSize({260.f, 30.f});
    setGeneratorButton.setFillColor(sf::Color(90, 90, 90));

    ENSURE(properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("RoadInfoPanel"));
}

bool RoadInfoPanel::properlyInitialized() const {
    return _initCheck == this;
}

void RoadInfoPanel::setFont(sf::Font& f) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("RoadInfoPanel"));
    REQUIRE(f.getInfo().family != "", ErrorMessagesInstructions::missingParameter("font"));

    font = &f;

    vehicleTypeLabel.setFont(f);
    vehicleSpawnLabel.setFont(f);
    setGeneratorLabel.setFont(f);

    vehicleTypeLabel.setCharacterSize(16);
    vehicleSpawnLabel.setCharacterSize(16);
    setGeneratorLabel.setCharacterSize(16);

    vehicleTypeLabel.setString("Change Type");
    vehicleSpawnLabel.setString("Spawn Vehicle");
    setGeneratorLabel.setString("Add Generator");

    ENSURE(font == &f, ErrorMessagesInstructions::notUpdatedSuccessfully("font"));
}

void RoadInfoPanel::setRoad(Road* road) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("RoadInfoPanel"));
    if (road) {
        REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));
    }
    selectedRoad = road;

    ENSURE(selectedRoad == road, ErrorMessagesInstructions::notUpdatedSuccessfully("selectedRoad"));
}

void RoadInfoPanel::draw(sf::RenderWindow& window, sf::Vector2u winSize) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("RoadInfoPanel"));
    REQUIRE(font != nullptr, ErrorMessagesInstructions::missingParameter("font"));

    if (!selectedRoad || !font) return;

    float panelWidth = 300.f;
    float panelX = static_cast<float>(winSize.x) - panelWidth;
    float panelY = 100.f;
    float panelHeight = static_cast<float>(winSize.y) - panelY - 30.f;

    panelBackground.setPosition(panelX, panelY);
    panelBackground.setSize({panelWidth, panelHeight});
    panelBackground.setFillColor(sf::Color(40, 40, 40, 235));
    panelBackground.setOutlineThickness(2.f);
    panelBackground.setOutlineColor(sf::Color(90, 90, 90));
    window.draw(panelBackground);

    float yOffset = panelY + 20.f;
    float xPadding = panelX + 20.f;

    sf::Text title("Road Details", *font, 26);
    title.setStyle(sf::Text::Bold);
    title.setPosition(xPadding, yOffset);
    title.setFillColor(sf::Color::White);
    window.draw(title);
    yOffset += 42.f;

    drawLabelPair(window, "Name", selectedRoad->getName(), yOffset, xPadding);
    drawLabelPair(window, "Length", std::to_string(selectedRoad->getLength()) + " m", yOffset, xPadding);
    drawLabelPair(window, "Vehicle Count", std::to_string(selectedRoad->getVehicles().size()), yOffset, xPadding);

    yOffset += 10.f;

    sf::Text section("Vehicle Generator", *font, 20);
    section.setStyle(sf::Text::Bold | sf::Text::Underlined);
    section.setPosition(xPadding, yOffset);
    section.setFillColor(sf::Color::White);
    window.draw(section);
    yOffset += 34.f;

    if (selectedRoad->hasVehicleGenerator()) {
        auto* gen = selectedRoad->getVehicleGenerator();
        drawLabelPair(window, "Generating Type", VehicleGenerator::to_string(gen->getVehicleType()), yOffset, xPadding);

        vehicleTypeButton.setPosition(xPadding, yOffset);
        vehicleSpawnButton.setPosition(xPadding + 150.f, yOffset);
        window.draw(vehicleTypeButton);
        window.draw(vehicleSpawnButton);

        // Center text in buttons
        auto centerText = [](sf::Text& text, const sf::RectangleShape& btn) {
            auto bounds = text.getLocalBounds();
            auto pos = btn.getPosition();
            auto size = btn.getSize();
            text.setPosition(pos.x + (size.x - bounds.width) / 2.f, pos.y + (size.y - bounds.height) / 2.f - 2.f);
        };

        centerText(vehicleTypeLabel, vehicleTypeButton);
        centerText(vehicleSpawnLabel, vehicleSpawnButton);

        window.draw(vehicleTypeLabel);
        window.draw(vehicleSpawnLabel);

        yOffset += vehicleTypeButton.getSize().y + 10.f;
    } else {
        if (roadStartsAtIntersection(selectedRoad)) {
            sf::Text warning("Cannot place generator:\nstarts at intersection", *font, 16);
            warning.setFillColor(sf::Color(255, 100, 100));
            warning.setPosition(xPadding, yOffset + 10.f);
            window.draw(warning);
            yOffset += 60.f;
        } else {
            sf::Text info("No active generator", *font, 16);
            info.setFillColor(sf::Color(180, 180, 180));
            info.setPosition(xPadding, yOffset + 10.f);
            window.draw(info);

            // Show add button
            setGeneratorButton.setPosition(xPadding, yOffset + 50.f);
            window.draw(setGeneratorButton);

            auto bounds = setGeneratorLabel.getLocalBounds();
            setGeneratorLabel.setPosition(
                setGeneratorButton.getPosition().x + (setGeneratorButton.getSize().x - bounds.width) / 2.f,
                setGeneratorButton.getPosition().y + (setGeneratorButton.getSize().y - bounds.height) / 2.f - 3.f
            );
            window.draw(setGeneratorLabel);
        }
    }
}

bool RoadInfoPanel::roadStartsAtIntersection(Road* road) {
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(road->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Road"));

    for (const auto& binding : road->getIntersectionBindings()) {
        if (binding.position == 0) return true;
    }
    return false;
}

void RoadInfoPanel::drawLabelPair(sf::RenderWindow& window, const std::string& label, const std::string& value, float& yOffset, float xPadding) {
    REQUIRE(font != nullptr, ErrorMessagesInstructions::missingParameter("font"));

    sf::Text lbl(label, *font, 16);
    lbl.setPosition(xPadding, yOffset);
    lbl.setFillColor(sf::Color(140, 140, 140));
    window.draw(lbl);
    yOffset += 20.f;

    sf::Text val(value, *font, 18);
    val.setStyle(sf::Text::Bold);
    val.setPosition(xPadding, yOffset);
    val.setFillColor(sf::Color(220, 220, 220));
    window.draw(val);
    yOffset += 28.f;
}

sf::RectangleShape& RoadInfoPanel::getSetGeneratorButton() { return setGeneratorButton; }
sf::RectangleShape& RoadInfoPanel::getVehicleTypeButton() { return vehicleTypeButton; }
sf::RectangleShape& RoadInfoPanel::getVehicleSpawnButton() { return vehicleSpawnButton; }