//============================================================================
// Name        : Camera.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : SFML Camera class
//============================================================================
#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

/**
 * @brief SFML Camera class
 */
class Camera {
public:
    /**
     * @brief Constructor that initializes the offset
     * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Camera' must end in a correctly initialized state.")
     */
    Camera();

    /**
     * @brief Handles mouse dragging, changes the offset
     * @param mouseStart Position where the mouse started dragging
     * @param mouseCurrent Current mouse position during dragging
     * \n REQUIRE(this->properlyInitialized(), "Error: Camera's constructor did not initialize properly")
     */
    void handleDrag(const sf::Vector2f& mouseStart, const sf::Vector2f& mouseCurrent);

    /**
     * @brief Returns the current offset of the camera
     * \n REQUIRE(this->properlyInitialized(), "Error: Camera's constructor did not initialize properly")
     */
    [[nodiscard]] sf::Vector2f getOffset() const;

    /**
     * @brief Moves the camera offset by a delta
     * \n REQUIRE(this->properlyInitialized(), "Error: Camera's constructor did not initialize properly")
     */
    void addOffset(const sf::Vector2f& delta);

    /**
     * @brief Checks whether the object is properly initialized
     * @return true if properly initialized
     */
    [[nodiscard]] bool properlyInitialized() const;

private:
    sf::Vector2f offset;
    Camera* _initCheck = nullptr;
};
#endif
