#include "Graphics/Animation.h"

// Splits the texture width into equal frames; height is the full sheet height.
sf::Vector2i Animation::frameSize(const sf::Texture& texture, int frameCount) {
    return { static_cast<int>(texture.getSize().x) / frameCount,
             static_cast<int>(texture.getSize().y) };
}

// Stores the strip layout and timing; frame state starts at zero.
Animation::Animation(sf::Vector2i frameSize, int frameCount, float frameDuration)
    : m_frameSize(frameSize)
    , m_frameCount(frameCount)
    , m_frameDuration(frameDuration) {
}

// Accumulates elapsed time and advances to the next frame when due, wrapping at the end.
void Animation::update(float dt) {
    m_timer += dt;
    if (m_timer < m_frameDuration)
        return;

    m_timer -= m_frameDuration;
    ++m_currentFrame;

    if (m_currentFrame >= m_frameCount)
        m_currentFrame = 0;
}

// Maps the current frame index to its rectangle on the horizontal strip.
sf::IntRect Animation::getFrameRect() const {
    return frameRect(m_currentFrame);
}

// Maps an arbitrary frame index to its rectangle on the horizontal strip.
sf::IntRect Animation::frameRect(int index) const {
    return sf::IntRect({ index * m_frameSize.x, 0 }, m_frameSize);
}

// Rewinds the animation to its first frame.
void Animation::reset() {
    m_currentFrame = 0;
    m_timer = 0.f;
}

