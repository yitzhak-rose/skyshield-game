#include "Utilities/Utils.h"
#include <random>

float Utils::randomFloat(float min, float max) {

    // Create the random engine once and reuse it for all calls.
    static std::mt19937 generator(std::random_device{}());

    // Define a rule that maps the engine's output evenly into [min, max].
    std::uniform_real_distribution<float> dist(min, max);

    // Ask the distribution for one number and return it.
    return dist(generator);
}

void GraphicsUtils::centerOrigin(sf::Sprite& sprite) {
    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                       bounds.position.y + bounds.size.y / 2.f });
}

void GraphicsUtils::centerOrigin(sf::Text& text) {
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                     bounds.position.y + bounds.size.y / 2.f });
}
