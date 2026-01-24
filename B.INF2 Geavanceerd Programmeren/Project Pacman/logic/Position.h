/**
 * @file Position.h
 * @brief Spatial structures and operators for 2D positioning and collision boxes.
 */

#ifndef PACMAN_POSITION_H
#define PACMAN_POSITION_H

#include <iosfwd>

/**
 * @struct Position
 * @brief Represents a 2D coordinate point using double precision.
 */
struct Position {
    // --- Member Variables ---
    double x; /**< The horizontal coordinate. */
    double y; /**< The vertical coordinate. */

    // --- Constructors ---

    /**
     * @brief Constructs a Position with specific coordinates.
     * @param x The initial x-coordinate.
     * @param y The initial y-coordinate.
     */
    Position(const double x, const double y) : x(x), y(y) {}

    // --- Compound Assignment Operators ---

    Position& operator+=(const Position& other);
    Position& operator-=(const Position& other);

    /** @brief Performs component-wise multiplication. */
    Position& operator*=(const Position& other);
    /** @brief Performs component-wise division. */
    Position& operator/=(const Position& other);

    /** @brief Scales both coordinates by a scalar factor. */
    Position& operator*=(double scalar);
    /** @brief Divides both coordinates by a scalar factor. */
    Position& operator/=(double scalar);

    // --- Utility Methods ---

    /**
     * @brief Maps a position from one bounding range to another.
     * @param current_min The minimum corner of the current coordinate system.
     * @param current_max The maximum corner of the current coordinate system.
     * @param wanted_min The minimum corner of the target coordinate system.
     * @param wanted_max The maximum corner of the target coordinate system.
     * @return A new Position mapped to the new range.
     */
    [[nodiscard]] Position rescale(const Position& current_min, const Position& current_max, const Position& wanted_min,
                                   const Position& wanted_max) const;

    /**
     * @brief Calculates the Manhattan (L1) distance to another position.
     * @param other The target position.
     * @return The absolute distance along the axes.
     */
    [[nodiscard]] double manhattan_distance_to(const Position& other) const;
};

// --- Non-Member Arithmetic Operators ---

Position operator*(const Position& lhs, const Position& rhs);
Position operator*(const Position& lhs, double scalar);
Position operator*(double scalar, const Position& rhs);
Position operator+(const Position& lhs, const Position& rhs);
Position operator-(const Position& lhs, const Position& rhs);
Position operator/(const Position& lhs, const Position& rhs);
Position operator/(const Position& lhs, double scalar);

/** @brief Checks for equality between two positions. */
bool operator==(const Position& lhs, const Position& rhs);

/** @brief Formats the position for stream output (e.g., "(x, y)"). */
std::ostream& operator<<(std::ostream& os, const Position& p);

/**
 * @struct Rectangle
 * @brief Defines an axis-aligned bounding box (AABB) using two Positions.
 */
struct Rectangle {
    Position topLeft;     /**< Coordinates of the upper-left corner. */
    Position bottomRight; /**< Coordinates of the lower-right corner. */

    /** @brief Constructs a Rectangle from specific corners. */
    Rectangle(const Position& top_left, const Position& bottom_right);

    /** @brief Constructs a Rectangle at (0,0) with zero size. */
    Rectangle();

    /** @return The geometric center of the rectangle. */
    [[nodiscard]] Position getCenter() const;

    /** @return A new Rectangle offset by the given Position. */
    [[nodiscard]] Rectangle movedBy(const Position& other) const;

    /** @return A new Rectangle offset by deltaX and deltaY. */
    [[nodiscard]] Rectangle movedBy(double deltaX, double deltaY) const;

    /** @brief Rescales the rectangle's bounds to a new coordinate system. */
    [[nodiscard]] Rectangle rescale(const Position& current_min, const Position& current_max,
                                    const Position& wanted_min, const Position& wanted_max) const;

    /** @return A new Rectangle scaled in size relative to its current top-left. */
    [[nodiscard]] Rectangle scaledBy(double scale) const;

    /** @brief Offsets the rectangle's position in-place. */
    void moveBy(const Position& other);
    void moveBy(double deltaX, double deltaY);

    /** @brief Relocates the rectangle so its top-left corner is at newTopLeft. */
    void moveTo(const Position& newTopLeft);

    /** @brief Rounds coordinates to the nearest integer grid point. */
    void snapToGrid();
};

#endif // PACMAN_POSITION_H