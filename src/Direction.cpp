#include "Enums/Direction.h"

#include <stdexcept>

Direction opposite(Direction dir)
{
    switch (dir)
    {
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Right;
        case Direction::Stay:
            return Direction::Stay;
        default:
            throw std::runtime_error("Unknown direction");
    }
}

sf::Vector2f toVector(Direction dir)
{
    switch (dir)
    {
        case Direction::Right:
            return { 1, 0 };
        case Direction::Left:
            return { -1, 0 };
        case Direction::Stay:
            return { 0, 0 };
        default:
            throw std::runtime_error("Unknown direction");
    }
}