#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstddef>

namespace MenuBarConfig {
    inline constexpr unsigned int TextSize = 32;
    inline constexpr unsigned int TitleTextSize = 42;
    inline constexpr unsigned int LevelTextSize = 28;
    inline constexpr float SidePadding = 28.f;
    inline constexpr float IconSize = 46.f;
    inline constexpr float BatteryHeight = 46.f;
    inline constexpr float PauseButtonSize = 48.f;
    inline constexpr float PauseIconWidth = 7.f;
    inline constexpr float PauseIconHeight = 24.f;
    inline constexpr float PauseIconGap = 8.f;
    inline constexpr float PauseBatteryGap = 18.f;
    inline constexpr float BombTextGap = 14.f;
    inline constexpr float LevelBatteryGap = 24.f;
    inline constexpr float VehicleArmorBarX = 390.f;
    inline constexpr float VehicleArmorBarWidth = 260.f;
    inline constexpr float VehicleArmorBarHeight = 18.f;
    inline constexpr float VehicleArmorBarOutlineThickness = 2.f;
    inline constexpr unsigned int VehicleArmorTextSize = 18;
    inline const sf::Color BackgroundColor(16, 20, 28, 220);
    inline const sf::Color TitleColor(244, 232, 178);
    inline const sf::Color LevelTextColor(235, 238, 245);
    inline const sf::Color VehicleArmorTrackColor(42, 48, 58, 230);
    inline const sf::Color VehicleArmorOutlineColor(235, 238, 245, 180);
    inline const sf::Color VehicleArmorHealthyColor(67, 176, 113);
    inline const sf::Color VehicleArmorWarningColor(235, 168, 66);
    inline const sf::Color VehicleArmorCriticalColor(211, 77, 83);
    inline const sf::Color PauseButtonColor(67, 176, 113, 220);
    inline const sf::Color PauseButtonOutlineColor(154, 230, 180, 235);
    inline const sf::Color PauseIconColor(255, 255, 255);
    inline constexpr float Height = 76.f;
    inline constexpr float CoinIconX = 260.f;
}

namespace OverlayViewConfig {
    inline constexpr std::size_t MaxActions = 3;
    inline constexpr float PanelWidth = 660.f;
    inline constexpr float PanelHeight = 360.f;
    inline constexpr float PanelOutlineThickness = 3.f;
    inline constexpr float ButtonWidth = 220.f;
    inline constexpr float ButtonHeight = 64.f;
    inline constexpr float ButtonGap = 30.f;
    inline constexpr float ButtonTopOffset = 82.f;
    inline constexpr float TitleTopOffset = -112.f;
    inline constexpr float BodyTopOffset = -35.f;
    inline constexpr float BodyLineSpacing = 38.f;
    inline constexpr float ButtonOutlineThickness = 2.f;
    inline constexpr float SelectedButtonOutlineThickness = 4.f;
    inline constexpr int SelectedColorBoost = 24;
    inline constexpr unsigned int TitleTextSize = 48;
    inline constexpr unsigned int BodyTextSize = 28;
    inline constexpr unsigned int ButtonTextSize = 30;
    inline const sf::Color backgroundDimColor(0, 0, 0, 135);
    inline const sf::Color PanelColor(16, 20, 28, 235);
    inline const sf::Color PanelOutlineColor(244, 232, 178, 210);
    inline const sf::Color TitleColor(244, 232, 178);
    inline const sf::Color BodyColor(235, 238, 245);
    inline const sf::Color ButtonTextColor(255, 255, 255);
    inline const sf::Color ButtonOutlineColor(255, 255, 255, 220);
    inline const sf::Color DefaultButtonColor(58, 68, 86);
    inline const sf::Color DisabledButtonColor(74, 78, 86);
    inline const sf::Color PrimaryButtonColor(49, 119, 88);
    inline const sf::Color DangerButtonColor(145, 61, 65);
}

namespace StartScreenConfig {
    inline constexpr float SettingsPadding = 24.f;
    inline constexpr float SettingsButtonSize = 78.f;
    inline constexpr float SettingsIconSize = 48.f;
    inline constexpr float QuitButtonSize = 78.f;
    inline constexpr float QuitIconSize = 48.f;
    inline constexpr float CoinCounterWidth = 210.f;
    inline constexpr float ButtonWidth = 360.f;
    inline constexpr float ButtonHeight = 72.f;
    inline constexpr float ButtonGap = 28.f;
    inline constexpr float ButtonOutlineThickness = 2.f;
    inline constexpr unsigned int ButtonTextSize = 36;
    inline const sf::Color ButtonColor(16, 20, 28, 175);
    inline const sf::Color ButtonHoverColor(49, 119, 88, 220);
    inline const sf::Color ButtonOutlineColor(244, 232, 178, 215);
    inline const sf::Color ButtonTextColor(255, 255, 255);
    inline const sf::Color SettingsButtonColor(16, 20, 28, 185);
    inline const sf::Color SettingsButtonHoverColor(49, 119, 88, 225);
    inline const sf::Color QuitButtonColor(145, 61, 65, 225);
    inline const sf::Color QuitButtonHoverColor(210, 62, 70, 245);
    inline const sf::Color QuitIconColor(255, 255, 255);
}

namespace ShopScreenConfig {
    namespace Layout {
        inline constexpr float TopPadding = 28.f;
        inline constexpr float SidePadding = 42.f;
        inline constexpr float BackButtonWidth = 170.f;
        inline constexpr float BackButtonHeight = 58.f;
        inline constexpr float CategoryTop = 110.f;
        inline constexpr float CategoryWidth = 215.f;
        inline constexpr float CategoryHeight = 56.f;
        inline constexpr float CategoryGap = 18.f;
        inline constexpr float GridTop = 205.f;
        inline constexpr float TileWidth = 300.f;
        inline constexpr float TileHeight = 365.f;
        inline constexpr float TileGap = 34.f;
        inline constexpr float ThumbnailSize = 220.f;
        inline constexpr float LockSize = 74.f;
        inline constexpr float WarningDotRadius = 11.f;
    }

    namespace Text {
        inline constexpr unsigned int TitleTextSize = 52;
        inline constexpr unsigned int ButtonTextSize = 28;
        inline constexpr unsigned int TileNameSize = 22;
        inline constexpr unsigned int TilePriceSize = 18;
    }

    namespace Colors {
        inline const sf::Color BackgroundColor(11, 15, 22, 235);
        inline const sf::Color PanelColor(16, 20, 28, 190);
        inline const sf::Color PanelHoverColor(34, 45, 60, 220);
        inline const sf::Color SelectedColor(49, 119, 88, 230);
        inline const sf::Color OutlineColor(244, 232, 178, 205);
        inline const sf::Color TextColor(245, 247, 252);
        inline const sf::Color MutedTextColor(190, 198, 210);
        inline const sf::Color LockedColor(255, 255, 255, 110);
        inline const sf::Color WarningColor(235, 139, 39);
    }

    using namespace Layout;
    using namespace Text;
    using namespace Colors;
}
