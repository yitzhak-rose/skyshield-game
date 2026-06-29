#pragma once

/// @brief App-level command requested by a screen.
enum class ScreenCommand {
    None,
    StartGame,
    OpenShop,
    OpenSettings,
    OpenHelp,
    ReturnToSettings,
    ReturnToStart,
    ResetUserData,
    Quit
};
