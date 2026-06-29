#pragma once

#include "Enums/OverlayActionType.h"

#include <functional>
#include <string>

/// @brief A labeled overlay button with optional styling and behavior.
struct OverlayAction {
    std::string label;
    OverlayActionType type = OverlayActionType::Default;
    std::function<void()> action;
    bool enabled = true;
};
