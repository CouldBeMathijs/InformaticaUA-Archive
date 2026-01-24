#include "Position.h"

#include "LogicConstants.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

/**
 * @brief Compound addition operator (this += other).
 * @param other The Position to add.
 * @return Reference to the current Position object.
 */
Position& Position::operator+=(const Position& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

/**
 * @brief Compound subtraction operator (this -= other).
 * @param other The Position to subtract.
 * @return Reference to the current Position object.
 */
Position& Position::operator-=(const Position& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

/**
 * @brief Compound component-wise multiplication operator (this *= other).
 * @param other The Position whose components to multiply by.
 * @return Reference to the current Position object.
 */
Position& Position::operator*=(const Position& other) {
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}

/**
 * @brief Compound scalar multiplication operator (this *= scalar).
 * @param scalar The scalar value to multiply by.
 * @return Reference to the current Position object.
 */
Position& Position::operator*=(double scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

/**
 * @brief Compound component-wise division operator (this /= other).
 * @param other The Position whose components to divide by.
 * @return Reference to the current Position object.
 */
Position& Position::operator/=(const Position& other) {
    if (other.x == 0.0 || other.y == 0.0) {
        throw std::runtime_error("Cannot divide by Position with zero component in operator/=.");
    }
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}

/**
 * @brief Compound scalar division operator (this /= scalar).
 * @param scalar The scalar value to divide by.
 * @return Reference to the current Position object.
 */
Position& Position::operator/=(const double scalar) {
    if (scalar == 0.0) {
        throw std::runtime_error("Cannot divide Position by scalar zero in operator/=");
    }
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

// --- Utility Method Definition ---

/**
 * @brief Rescales the current Position from one range to another.
 * @param current_min The minimum bound of the current range.
 * @param current_max The maximum bound of the current range.
 * @param wanted_min The minimum bound of the desired range.
 * @param wanted_max The maximum bound of the desired range.
 * @return The rescaled Position.
 */
Position Position::rescale(const Position& current_min, const Position& current_max, const Position& wanted_min,
                           const Position& wanted_max) const {
    const Position range_current = current_max - current_min;

    // Check for zero range to prevent division by zero in the normalization step
    if (range_current.x == 0.0 && range_current.y == 0.0) {
        throw std::runtime_error("Current range has zero size.");
    }

    // Step 1: Normalize the position to a [0, 1] range
    // Normalized = (current - min) / range
    const Position normalized = (*this - current_min) / range_current;

    // Step 2: Scale the normalized position to the wanted range
    // Output = (Normalized * wanted_range) + wanted_min
    const Position range_wanted = wanted_max - wanted_min;
    const Position out = (normalized * range_wanted) + wanted_min;

    return out;
}

double Position::manhattan_distance_to(const Position& other) const {
    return std::abs(this->x - other.x) + std::abs(this->y - other.y);
}

// --- Non-Member Operator Definitions ---

/**
 * @brief Component-wise addition operator (lhs + rhs).
 */
Position operator+(const Position& lhs, const Position& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }

/**
 * @brief Component-wise subtraction operator (lhs - rhs).
 */
Position operator-(const Position& lhs, const Position& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

/**
 * @brief Component-wise multiplication operator (lhs * rhs).
 */
Position operator*(const Position& lhs, const Position& rhs) { return {lhs.x * rhs.x, lhs.y * rhs.y}; }

/**
 * @brief Scalar multiplication (Position * scalar).
 */
Position operator*(const Position& lhs, double scalar) { return {lhs.x * scalar, lhs.y * scalar}; }

/**
 * @brief Scalar multiplication (scalar * Position).
 */
Position operator*(double scalar, const Position& rhs) { return {scalar * rhs.x, scalar * rhs.y}; }

/**
 * @brief Component-wise division operator (lhs / rhs).
 */
Position operator/(const Position& lhs, const Position& rhs) {
    // Check for division by zero on either component
    if (rhs.x == 0.0 || rhs.y == 0.0) {
        throw std::runtime_error("Cannot divide by Position with zero component");
    }
    return {lhs.x / rhs.x, lhs.y / rhs.y};
}

/**
 * @brief Scalar division (Position / scalar).
 */
Position operator/(const Position& lhs, double scalar) {
    if (scalar == 0.0) {
        throw std::runtime_error("Cannot divide Position by scalar zero");
    }
    return {lhs.x / scalar, lhs.y / scalar};
}

/**
 * @brief Equality comparison operator.
 */
bool operator==(const Position& lhs, const Position& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

/**
 * @brief Stream insertion operator for easy printing.
 */
std::ostream& operator<<(std::ostream& os, const Position& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

Rectangle::Rectangle(const Position& top_left, const Position& bottom_right)
    : topLeft(top_left), bottomRight(bottom_right) {}

Rectangle::Rectangle()
    : topLeft{std::numeric_limits<float>::signaling_NaN(), std::numeric_limits<float>::signaling_NaN()},
      bottomRight(topLeft) {}

void Rectangle::moveTo(const Position& newTopLeft) {
    // Calculate the current width and height of the rectangle
    double width = bottomRight.x - topLeft.x;
    double height = bottomRight.y - topLeft.y; // Assumes y increases downwards

    // Update the topLeft position
    topLeft = newTopLeft;

    // Use the calculated width/height to determine the new bottomRight position
    bottomRight.x = newTopLeft.x + width;
    bottomRight.y = newTopLeft.y + height;
}

void Rectangle::moveBy(const Position& other) {
    topLeft += other;
    bottomRight += other;
}

Rectangle Rectangle::movedBy(const Position& other) const {
    Rectangle out = *this;
    out.moveBy(other);
    return out;
}

void Rectangle::moveBy(const double deltaX, const double deltaY) {
    // Apply the delta to the x-coordinate of both corners
    topLeft.x += deltaX;
    bottomRight.x += deltaX;

    // Apply the delta to the y-coordinate of both corners
    topLeft.y += deltaY;
    bottomRight.y += deltaY;
}

Rectangle Rectangle::movedBy(const double deltaX, const double deltaY) const {
    Rectangle out = *this;
    out.moveBy(deltaX, deltaY);
    return out;
}

Rectangle Rectangle::scaledBy(double scale) const {

    scale = std::abs(scale);
    const double center_x = (topLeft.x + bottomRight.x) / 2.0;
    const double center_y = (topLeft.y + bottomRight.y) / 2.0;

    const double current_width = bottomRight.x - topLeft.x;
    const double current_height = bottomRight.y - topLeft.y;

    const double new_width = current_width * scale;
    const double new_height = current_height * scale;

    const double new_half_width = new_width / 2.0;
    const double new_half_height = new_height / 2.0;

    Rectangle scaled_rectangle;

    scaled_rectangle.topLeft.x = center_x - new_half_width;
    scaled_rectangle.topLeft.y = center_y - new_half_height;
    scaled_rectangle.bottomRight.x = center_x + new_half_width;
    scaled_rectangle.bottomRight.y = center_y + new_half_height;

    return scaled_rectangle;
}

Position Rectangle::getCenter() const { return {(topLeft + bottomRight) / 2}; }

Rectangle Rectangle::rescale(const Position& current_min, const Position& current_max, const Position& wanted_min,
                             const Position& wanted_max) const {
    return {topLeft.rescale(current_min, current_max, wanted_min, wanted_max),
            bottomRight.rescale(current_min, current_max, wanted_min, wanted_max)};
}

void Rectangle::snapToGrid() {
    constexpr double epsilon = LogicConstants::TILE_HEIGHT / 7;

    // --- 1. Calculate Half Tile Dimensions ---
    constexpr double HALF_TILE_WIDTH = 2.0 / (2.0 * LogicConstants::AMOUNT_OF_TILES_WIDTH);
    constexpr double HALF_TILE_HEIGHT = 2.0 / (2.0 * LogicConstants::AMOUNT_OF_TILES_HEIGHT);

    // --- 2. Calculate Center Coordinates ---
    const double centerX = (topLeft.x + bottomRight.x) / 2.0;
    const double centerY = (topLeft.y + bottomRight.y) / 2.0;

    // --- 3. Determine Tile Coordinates (X-axis) ---
    // deltaX is the distance from the left boundary (-1.0)
    const double deltaX = centerX - (-1.0);
    // unitsX is the number of half-tile widths from the left boundary
    const double unitsX = deltaX / HALF_TILE_WIDTH;

    // The grid centers are at odd multiples of HALF_TILE_WIDTH
    // (1 * HALF_TILE_WIDTH, 3 * HALF_TILE_WIDTH, 5 * HALF_TILE_WIDTH, ...)

    // --- 4. Check for Almost Centered (X-axis) ---
    // Check if the current unitsX is close to an odd unit center
    if (const double nearestOddUnitsX = std::round(unitsX - 1.0 / 2.0) * 2.0 + 1.0;
        std::abs(unitsX - nearestOddUnitsX) <= epsilon) {
        // --- 5. Calculate Target Center (X-axis) ---
        // Calculate the center position of the target tile in world coordinates
        const double targetDeltaX = nearestOddUnitsX * HALF_TILE_WIDTH;
        const double targetCenterX = -1.0 + targetDeltaX;

        // --- 6. Move the Rectangle (X-axis) ---
        // Calculate the offset required to move the center to targetCenterX
        const double offsetX = targetCenterX - centerX;

        // Apply the offset to the rectangle's coordinates
        topLeft.x += offsetX;
        bottomRight.x += offsetX;
    }

    // --- 3. Determine Tile Coordinates (Y-axis) ---
    // deltaY is the distance from the top boundary (1.0)
    const double deltaY = 1.0 - centerY;
    // unitsY is the number of half-tile heights from the top boundary
    const double unitsY = deltaY / HALF_TILE_HEIGHT;

    // The grid centers are at odd multiples of HALF_TILE_HEIGHT
    // (1 * HALF_TILE_HEIGHT, 3 * HALF_TILE_HEIGHT, 5 * HALF_TILE_HEIGHT, ...)

    // --- 4. Check for Almost Centered (Y-axis) ---
    // Check if the current unitsY is close to an odd unit center
    if (const double nearestOddUnitsY = std::round((unitsY - 1.0) / 2.0) * 2.0 + 1.0;
        std::abs(unitsY - nearestOddUnitsY) <= epsilon) {
        // --- 5. Calculate Target Center (Y-axis) ---
        // Calculate the center position of the target tile in world coordinates
        const double targetDeltaY = nearestOddUnitsY * HALF_TILE_HEIGHT;
        const double targetCenterY = 1.0 - targetDeltaY; // Note: 1.0 - deltaY for Y-axis

        // --- 6. Move the Rectangle (Y-axis) ---
        // Calculate the offset required to move the center to targetCenterY
        const double offsetY = targetCenterY - centerY;

        // Apply the offset to the rectangle's coordinates
        topLeft.y += offsetY;
        bottomRight.y += offsetY;
    }
}