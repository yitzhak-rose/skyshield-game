#pragma once

/// @brief User action requested by the settings UI for the settings manager to handle.
enum class SettingsAction {
    None,
    ReturnToStart,
    Mute,
    Unmute,
    OpenHelp,
    ResetUserData
};
