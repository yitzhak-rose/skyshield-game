#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"
#include "App/AssetConstants.h"

/// @brief The player-controlled drone. Moves horizontally, drops bombs, manages resources.
class Drone : public MovingObject {
public:
    /// @brief Constructs the drone with a texture and a starting position.
    Drone(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Updates the drone for one frame (movement, animation, battery drain).
    void update(float dt) override;

    /// @brief Consumes one bomb from the ammo count.
    bool consumeAmmo();

    /// @brief Marks the drone as the player (receives input and drops bombs).
    bool isPlayer() const override { return true; }

    /// @brief Hands the World a bomb when one was requested this frame.
    std::unique_ptr<GameObject> takeSpawn() override;

private:
    /// @brief Reads keyboard arrows and sets the drone's velocity.
    void handleInput();

    /// @brief Reads Space (edge-triggered) and queues a bomb if ammo allows.
    void handleBombDrop();

    /// @brief Starts moving the drone left.
    void moveLeft();

    /// @brief Starts moving the drone right.
    void moveRight();

    /// @brief Stops the drone's horizontal movement.
    void stop();

    /// @brief Cycles the drone's rotor frames.
    Animation m_animation;

    /// @brief Texture-specific scale that keeps every drone skin at the same on-screen height.
    float m_displayScale = 1.f;

    /// @brief True for the one frame a bomb should be created.
    bool m_bombRequested = false;

    /// @brief Tracks last frame's Space state for edge detection.
    bool m_spaceWasPressed = true;
};
