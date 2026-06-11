#include "Utils.h"
#include <random>


// Returns a random float between min and max.
float randomFloat(float min, float max) {

    // Create the random engine once and reuse it for all calls.
    static std::mt19937 generator(std::random_device{}());

    // Define a rule that maps the engine's output evenly into [min, max].
    std::uniform_real_distribution<float> dist(min, max);

    // Ask the distribution for one number and return it.
    return dist(generator);
}