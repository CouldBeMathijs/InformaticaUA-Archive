#include "Camera.h"
#include "SfmlConstants.h"

void resizeView(const sf::RenderWindow& window, sf::View& view, sf::FloatRect& currentViewport) {
    const sf::Vector2u windowSize = window.getSize();
    const float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    sf::FloatRect viewport;

    if (windowAspectRatio > LogicConstants::TARGET_ASPECT_RATIO) {
        const float width = LogicConstants::TARGET_ASPECT_RATIO / windowAspectRatio;
        const float x = (1.0f - width) / 2.0f; // Center the viewport horizontally
        viewport = sf::FloatRect(x, 0.0f, width, 1.0f);
    } else {
        const float height = windowAspectRatio / LogicConstants::TARGET_ASPECT_RATIO;
        const float y = (1.0f - height) / 2.0f; // Center the viewport vertically
        viewport = sf::FloatRect(0.0f, y, 1.0f, height);
    }

    view.setViewport(viewport);

    currentViewport = viewport;

    view.setSize(SfmlConstants::VIEW_WIDTH, SfmlConstants::VIEW_HEIGHT);
}

Camera& Camera::getInstance() {
    static Camera instance(sf::VideoMode(static_cast<unsigned int>(SfmlConstants::VIEW_WIDTH),
                                         static_cast<unsigned int>(SfmlConstants::VIEW_HEIGHT)),
                           "Pacman GP", sf::Style::Close | sf::Style::Resize);
    return instance;
}

Camera::Camera(const sf::VideoMode mode, const sf::String& title, const sf::Uint32 style)
    : m_window(mode, title, style),
      m_view(sf::FloatRect(0.f, 0.f, SfmlConstants::VIEW_WIDTH, SfmlConstants::VIEW_HEIGHT)) {
    resizeView(m_window, m_view, m_currentViewport);
    m_window.setView(m_view);
}

void Camera::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        resizeView(m_window, m_view, m_currentViewport);
        m_window.setView(m_view);
    }
}

sf::RenderWindow& Camera::getWindow() { return m_window; }

const sf::FloatRect& Camera::getViewport() const { return m_currentViewport; }

void Camera::resetView() { m_window.setView(m_view); }