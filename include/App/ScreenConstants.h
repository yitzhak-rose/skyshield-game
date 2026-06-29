#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstddef>
#include <string>

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

namespace SettingsScreenConfig {
    inline constexpr float Padding = 36.f;
    inline constexpr float BackButtonSize = 64.f;
    inline constexpr float ButtonOutlineThickness = 2.f;
    inline constexpr float ActiveButtonOutlineThickness = 4.f;
    inline constexpr float TitleY = 64.f;
    inline constexpr float PanelX = 350.f;
    inline constexpr float PanelY = 112.f;
    inline constexpr float PanelWidth = 1100.f;
    inline constexpr float PanelHeight = 560.f;
    inline constexpr float ButtonWidth = 330.f;
    inline constexpr float ButtonHeight = 62.f;
    inline constexpr float PairedButtonWidth = 300.f;
    inline constexpr float ButtonGap = 34.f;
    inline constexpr float AudioButtonWidth = 260.f;
    inline constexpr float AudioButtonHeight = 96.f;
    inline constexpr float AudioIconSize = 44.f;
    inline constexpr float AudioIconOffsetX = 26.f;
    inline constexpr float AudioIconOffsetY = 22.f;
    inline constexpr float AudioLabelOffsetX = 28.f;
    inline constexpr float AudioButtonY = 155.f;
    inline constexpr float SliderX = 595.f;
    inline constexpr float SliderY = 335.f;
    inline constexpr float SliderWidth = 610.f;
    inline constexpr float SliderHeight = 14.f;
    inline constexpr float SliderKnobRadius = 18.f;
    inline constexpr float VolumeIconGap = 30.f;
    inline constexpr float VolumeValueOffsetY = 58.f;
    inline constexpr float MaxVolume = 100.f;
    inline constexpr float VolumeIconSize = 46.f;
    inline constexpr float PairedButtonsY = 455.f;
    inline constexpr float ResetButtonY = 555.f;
    inline constexpr unsigned int TitleTextSize = 52;
    inline constexpr unsigned int LabelTextSize = 24;
    inline constexpr unsigned int ButtonTextSize = 26;
    inline constexpr unsigned int VolumeTextSize = 22;
    inline const sf::Color BackgroundColor(8, 12, 18, 220);
    inline const sf::Color PanelColor(16, 20, 28, 215);
    inline const sf::Color PanelOutlineColor(244, 232, 178, 210);
    inline const sf::Color ButtonColor(58, 68, 86, 230);
    inline const sf::Color ButtonHoverColor(49, 119, 88, 235);
    inline const sf::Color DangerColor(145, 61, 65, 235);
    inline const sf::Color DangerHoverColor(195, 70, 76, 245);
    inline const sf::Color TextColor(245, 247, 252);
    inline const sf::Color MutedTextColor(155, 163, 174);
    inline const sf::Color InactiveAudioButtonHoverColor(76, 88, 108, 235);
    inline const sf::Color SliderTrackColor(70, 80, 96);
    inline const sf::Color SliderFillColor(67, 176, 113);
    inline const sf::Color DisabledColor(92, 96, 104, 175);
    inline const std::string AttributionText =
        "Game Credits\n"
        "Story\n"
        "Yithak Rose\n"
        "\n"
        "Design\n"
        "Yitzhak Rose\n"
        "Benji Mandel\n"
        "\n"
        "Development\n"
        "Yithak Rose\n"
        "Benji Mandel\n"
        "\n"
        "Sounds\n"
        "YouTube\n"
        "Gaming Sound FX, Alec Koff Label\n"
        "Pixabay\n"
        "HitsLab sounds, Tuomas_Data, floraphonic\n"
        "\n"
        "Assets\n"
        "Gemini, ChatGPT, Claude";
}

namespace HelpScreenConfig {
    inline constexpr float Padding = 32.f;
    inline constexpr float BackButtonSize = 64.f;
    inline constexpr float TextPanelX = 180.f;
    inline constexpr float TextPanelY = 82.f;
    inline constexpr float TextPanelWidth = 1440.f;
    inline constexpr float TextPanelHeight = 590.f;
    inline constexpr float PanelOutlineThickness = 2.f;
    inline constexpr float CardOutlineThickness = 1.f;
    inline constexpr float IconSize = 52.f;
    inline constexpr float TitleY = 125.f;
    inline constexpr float GoalY = 172.f;
    inline constexpr float SectionHeadingY = 220.f;
    inline constexpr float LeftHeadingX = 510.f;
    inline constexpr float LeftIconX = 250.f;
    inline constexpr float LeftTextX = 430.f;
    inline constexpr float LeftIconSlotWidth = 118.f;
    inline constexpr float LeftTextWidth = 430.f;
    inline constexpr float LeftRowStartY = 250.f;
    inline constexpr float LeftRowGap = 65.f;
    inline constexpr float LeftTextOffsetY = 14.f;
    inline constexpr float HelpTextLineSpacing = 24.f;
    inline constexpr float IconGap = 12.f;
    inline constexpr float SpacebarExtraWidth = 14.f;
    inline constexpr float RightCardX = 1000.f;
    inline constexpr float RightCardWidth = 500.f;
    inline constexpr float RightHeadingX = RightCardX + RightCardWidth / 2.f;
    inline constexpr float RightTitleOffsetX = 28.f;
    inline constexpr float RightBodyOffsetX = 170.f;
    inline constexpr float RightTitleOffsetY = 25.f;
    inline constexpr float RightBodyOffsetY = 18.f;
    inline constexpr float RightBodyLineSpacing = 23.f;
    inline constexpr float GameFlowHeadingY = 220.f;
    inline constexpr float PauseCardY = 245.f;
    inline constexpr float ResetCardY = 338.f;
    inline constexpr float OutcomeHeadingY = 430.f;
    inline constexpr float WinCardY = 455.f;
    inline constexpr float LoseCardY = 548.f;
    inline constexpr float OutcomeCardHeight = 78.f;
    inline constexpr float LoseCardHeight = 96.f;
    inline constexpr float LoseHeadingOffsetY = 14.f;
    inline constexpr float LoseTextOffsetX = 34.f;
    inline constexpr float LoseTextStartY = 41.f;
    inline constexpr float LoseTextLineSpacing = 16.f;
    inline constexpr unsigned int TitleTextSize = 44;
    inline constexpr unsigned int SectionTextSize = 23;
    inline constexpr unsigned int BodyTextSize = 20;
    inline constexpr unsigned int LoseConditionTextSize = 15;
    inline const sf::Color DimColor(0, 0, 0, 118);
    inline const sf::Color PanelColor(10, 14, 20, 200);
    inline const sf::Color OutcomeCardColor(20, 27, 37, 170);
    inline const sf::Color LoseCardColor(37, 23, 28, 172);
    inline const sf::Color PanelOutlineColor(244, 232, 178, 185);
    inline const sf::Color TitleColor(244, 232, 178);
    inline const sf::Color TextColor(245, 247, 252);
    inline const sf::Color AccentColor(154, 230, 180);
    inline const sf::Color DangerColor(255, 179, 186);
}

namespace ScrollableOverlayConfig {
    inline constexpr float PanelWidth = 980.f;
    inline constexpr float PanelHeight = 540.f;
    inline constexpr float TextWidth = 820.f;
    inline constexpr float TextHeight = 318.f;
    inline constexpr float CloseButtonWidth = 220.f;
    inline constexpr float CloseButtonHeight = 54.f;
    inline constexpr float ScrollStep = 36.f;
    inline constexpr unsigned int TitleTextSize = 40;
    inline constexpr unsigned int BodyTextSize = 20;
    inline constexpr unsigned int HeadingTextSize = 28;
    inline constexpr unsigned int SubheadingTextSize = 22;
    inline constexpr unsigned int ButtonTextSize = 26;
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
