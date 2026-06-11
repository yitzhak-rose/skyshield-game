#pragma once

#include <SFML/Graphics.hpp>

/// @Brief Movement in this project is restricted to three horizontal options:
/// Left, Right, or Stay.
enum class Direction
{
    Left,
    Right,
    Stay,
    Max,
};

/// @brief Return the opposite of the given direction.
/// @param dir The input direction.
/// @return The opposite direction: Left <-> Right, Stay -> Stay.
Direction opposite(Direction dir);

/// @brief Convert a Direction to a unit movement vector (SFML coordinates).
/// @param dir The direction to convert.
/// @return An sf::Vector2f unit vector representing the direction:
///   Right -> {1, 0}
///   Left -> {-1, 0}
///   Stay -> {0, 0}.
sf::Vector2f toVector(Direction dir);
