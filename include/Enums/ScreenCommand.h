#pragma once

/// @brief App-level command requested by a screen.
enum class ScreenCommand {
    None,
    StartGame,
    OpenShop,
    ReturnToStart,
    Quit
};
