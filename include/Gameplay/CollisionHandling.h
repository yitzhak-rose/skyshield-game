#pragma once

class GameObject;

/// @brief Resolves a collision between two game objects using multiple dispatch:
///        the concrete pair of dynamic types selects the handler. A pair with no
///        registered handler simply does nothing (the objects do not interact).
void processCollision(GameObject& object1, GameObject& object2);