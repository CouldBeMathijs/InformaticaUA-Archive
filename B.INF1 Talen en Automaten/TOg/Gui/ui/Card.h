#pragma once
#include "UIElement.h"
#include <vector>
#include <memory>

class Card : public UIElement {
public:
    Card(const sf::Font& font, std::string title, std::string desc);
    void setPosition(sf::Vector2f pos) override;
    void setSize(sf::Vector2f size) override;
    void addChild(std::shared_ptr<UIElement> child);
    void draw(sf::RenderTarget& target) override;

    void handleEvent(const sf::Event& evt) override;

    std::string getTitle() const;
    const std::vector<std::shared_ptr<UIElement>>& getChildren() const;

    sf::Vector2f getPosition() const;

    sf::Vector2f getSize() const;

    template <typename T>
    void removeChildrenOfType() {
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [](const std::shared_ptr<UIElement>& child) {
                    return std::dynamic_pointer_cast<T>(child) != nullptr;
                }),
            children.end()
        );
    }

private:
    sf::RectangleShape background;
    sf::Text title, description;
    std::vector<std::shared_ptr<UIElement>> children;
    sf::Font const& font;
};