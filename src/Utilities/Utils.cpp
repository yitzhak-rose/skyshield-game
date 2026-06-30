#include "Utilities/Utils.h"

#include <algorithm>
#include <random>
#include <sstream>

float Utils::randomFloat(float min, float max) {

    // Create the random engine once and reuse it for all calls.
    static std::mt19937 generator(std::random_device{}());

    // Define a rule that maps the engine's output evenly into [min, max].
    std::uniform_real_distribution<float> dist(min, max);

    // Ask the distribution for one number and return it.
    return dist(generator);
}

bool GeometryUtils::contains(sf::FloatRect bounds, sf::Vector2f position) {
    return position.x >= bounds.position.x &&
        position.x <= bounds.position.x + bounds.size.x &&
        position.y >= bounds.position.y &&
        position.y <= bounds.position.y + bounds.size.y;
}

sf::Vector2f GeometryUtils::centerOf(sf::FloatRect bounds) {
    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}

sf::Vector2f GeometryUtils::centerOf(const sf::RectangleShape& shape) {
    return centerOf(shape.getGlobalBounds());
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

void GraphicsUtils::fitSpriteTo(sf::Sprite& sprite, sf::FloatRect bounds) {
    const sf::IntRect textureRect = sprite.getTextureRect();
    const sf::Vector2i size = textureRect.size;
    const float scale = std::min(
        bounds.size.x / static_cast<float>(size.x),
        bounds.size.y / static_cast<float>(size.y));
    sprite.setScale({ scale, scale });
    centerOrigin(sprite);
    sprite.setPosition(GeometryUtils::centerOf(bounds));
}

void GraphicsUtils::styleRectangle(sf::RectangleShape& shape,
                                   sf::Color fill,
                                   sf::Color outline,
                                   float outlineThickness) {
    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
    shape.setOutlineThickness(outlineThickness);
}

sf::Text TextUtils::makeText(const sf::Font& font,
                             const std::string& value,
                             unsigned int size,
                             sf::Vector2f position,
                             sf::Color color) {
    sf::Text text(font, value, size);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}

sf::Text TextUtils::makeCenteredText(const sf::Font& font,
                                     const std::string& value,
                                     unsigned int size,
                                     sf::Vector2f position,
                                     sf::Color color) {
    sf::Text text = makeText(font, value, size, position, color);
    GraphicsUtils::centerOrigin(text);
    return text;
}

std::vector<std::string> TextUtils::wrapTextLines(const sf::Font& font,
                                                  const std::string& text,
                                                  unsigned int size,
                                                  float maxWidth,
                                                  std::size_t maxLines) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string word;
    std::string current;

    while (stream >> word) {
        const std::string candidate = current.empty() ? word : current + " " + word;
        sf::Text candidateText(font, candidate, size);
        if (candidateText.getLocalBounds().size.x <= maxWidth || current.empty()) {
            current = candidate;
            continue;
        }

        if (maxLines > 0 && lines.size() + 1 >= maxLines) {
            current += " " + word;
            break;
        }

        lines.push_back(current);
        current = word;
    }

    if (!current.empty())
        lines.push_back(current);

    if (maxLines > 0 && lines.size() > maxLines) {
        lines.resize(maxLines);
    }

    if (maxLines > 0 && lines.size() == maxLines) {
        sf::Text lastLine(font, lines.back(), size);
        while (lastLine.getLocalBounds().size.x > maxWidth && lines.back().size() > 3) {
            lines.back().erase(lines.back().size() - 4);
            lines.back() += "...";
            lastLine.setString(lines.back());
        }
    }

    return lines;
}

void TextUtils::drawMultiline(sf::RenderTarget& target,
                              const sf::Font& font,
                              const std::vector<std::string>& lines,
                              unsigned int size,
                              sf::Vector2f position,
                              sf::Color color,
                              float lineSpacing) {
    for (std::size_t index = 0; index < lines.size(); ++index) {
        sf::Text line(font, lines[index], size);
        line.setFillColor(color);
        line.setPosition({ position.x, position.y + static_cast<float>(index) * lineSpacing });
        target.draw(line);
    }
}
