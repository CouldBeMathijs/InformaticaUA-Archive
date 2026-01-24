#ifndef PACMAN_CAMERA_H
#define PACMAN_CAMERA_H

#include <SFML/Graphics.hpp>

/**
 * @brief Singleton class in charge of the sfml Window and Game Viewport
 */
class Camera {
    sf::RenderWindow m_window;
    sf::View m_view;
    sf::FloatRect m_currentViewport;

    Camera(sf::VideoMode mode, const sf::String& title, sf::Uint32 style);

public:
    // -- Deleted constructors --
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    /**
     * @brief Singleton getter method
     * @return The one and only Camera instance
     */
    static Camera& getInstance();

    /**
     * @brief Resize the window on resize event
     * @param event Current event to handle, non-resize events will be ignored
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Getter for the sfml window
     * @return The sfml Window
     */
    sf::RenderWindow& getWindow();

    /**
     * @brief Getter for the viewport
     * Only part of the sfml window is to be used, the rest are black bars caused by resizing
     * @return The viewport in which the main game ought to be drawn
     */
    const sf::FloatRect& getViewport() const;

    /**
     * @brief Reset the view to the original view of the sfml window
     */
    void resetView();
};

#endif // PACMAN_CAMERA_H
