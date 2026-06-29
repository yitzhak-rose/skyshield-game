#pragma once
#include <SFML/Graphics.hpp>

/// @brief A reusable component that cycles through frames of a horizontal sprite sheet.
class Animation {
public:

    /// @brief Constructs an animation from a uniform horizontal strip of frames.
    Animation(sf::Vector2i frameSize, int frameCount, float frameDuration);

    /// @brief Advances the animation by one frame's worth of elapsed time.
    void update(float dt);

    /// @brief Returns the texture sub-rectangle of the current frame.
    sf::IntRect getFrameRect() const;

    /// @brief Returns the texture sub-rectangle of a specific frame index.
    sf::IntRect frameRect(int index) const;

    /// @brief Sets the sprite's texture rect to the first frame of the animation.
    void applyInitialFrame(sf::Sprite& sprite) const;

    /// @brief Advances the animation and applies the current frame to a sprite.
    void apply(sf::Sprite& sprite, float dt);

    /// @brief Rewinds the animation to its first frame.
    void reset();

    /// @brief Splits a sprite-sheet texture into equal-width frames of full height.
    static sf::Vector2i frameSize(const sf::Texture& texture, int frameCount);

private:

    /// @brief The size of a single frame in texture pixels.
    sf::Vector2i m_frameSize;

    /// @brief How many frames the strip contains.
    int m_frameCount;

    /// @brief How long each frame stays on screen, in seconds.
    float m_frameDuration;

    /// @brief The frame currently being shown.
    int m_currentFrame = 0;

    /// @brief Time accumulated toward the next frame switch.
    float m_timer = 0.f;
};