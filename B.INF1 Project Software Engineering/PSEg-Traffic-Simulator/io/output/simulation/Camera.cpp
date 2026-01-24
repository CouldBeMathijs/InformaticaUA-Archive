#include "Camera.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"

Camera::Camera()
        : offset(0.f, 0.f) {
    _initCheck = this;
    ENSURE(properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("Camera"));
}

void Camera::handleDrag(const sf::Vector2f& mouseStart, const sf::Vector2f& mouseCurrent) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Camera"));
    offset += (mouseCurrent - mouseStart);
}

void Camera::addOffset(const sf::Vector2f& delta) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Camera"));
    offset += delta;
}

sf::Vector2f Camera::getOffset() const {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("Camera"));
    return offset;
}

bool Camera::properlyInitialized() const {
    return _initCheck == this;
}
