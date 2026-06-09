#pragma once
#include "GameObject.h"
#include "Constants.h"

// The armored command vehicle the player protects. Static (inherits GameObject).
class CommandVehicle : public GameObject
{
public:
    // Constructs the vehicle with a texture and a starting position.
    CommandVehicle(const sf::Texture &texture, sf::Vector2f position);

    // Updates the vehicle for one frame.
    void update(float dt) override;

    // Reduces the armor by one hit's worth of damage.
    void takeDamage();

    // Returns the current armor as a percentage (0 to 100).
    float getArmorPercent() const { return m_armor; }

private:
    float m_armor = VehicleConfig::StartArmor;
};