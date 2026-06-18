#pragma once

/// @brief Interface for objects that are updated once per frame.
class IUpdatable {
public:
    /// @brief Virtual destructor.
    virtual ~IUpdatable() = default;

    /// @brief Advances the object's state by one frame.
    virtual void update(float dt) = 0;
};