#pragma once
#include <memory>

class GameObject;

class ISpawner {
public:
    virtual ~ISpawner() = default;

    virtual std::unique_ptr<GameObject> takeSpawn() = 0;

    virtual bool isAlive() const = 0;
};