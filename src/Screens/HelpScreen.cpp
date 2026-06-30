#include "Screens/HelpScreen.h"

#include "App/Constants.h"
#include "App/AssetConstants.h"
#include "App/ScreenConstants.h"
#include "Resources/ResourceManager.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <array>

namespace {
int helpIconFrameCount(GameObjects icon) {
    switch (icon) {
        case GameObjects::Bomb:
            return BombConfig::Sprite.frameCount;
        case GameObjects::Coin:
            return CoinConfig::Sprite.frameCount;
        case GameObjects::CommandCar:
            return VehicleConfig::Sprite.frameCount;
        case GameObjects::Drone:
            return DroneConfig::Sprite.frameCount;
        default:
            return 1;
    }
}

void showFirstFrame(sf::Sprite& sprite, GameObjects icon) {
    const int frameCount = helpIconFrameCount(icon);
    if (frameCount <= 1)
        return;

    const sf::Vector2u textureSize = sprite.getTexture().getSize();
    sprite.setTextureRect({
        { 0, 0 },
        { static_cast<int>(textureSize.x) / frameCount, static_cast<int>(textureSize.y) }
    });
}
}

HelpScreen::HelpScreen()
    : m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::HelpBackground))
    , m_dim({ WindowConfig::Width, WindowConfig::Height })
    , m_panel({ HelpScreenConfig::TextPanelWidth, HelpScreenConfig::TextPanelHeight })
    , m_backSprite(ResourceManager::instance().getImage(GameObjects::ChevronBack))
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce)) {
    configureLayout();
}

void HelpScreen::configureLayout() {
    configureBackground();
    m_dim.setFillColor(HelpScreenConfig::DimColor);
    m_panel.setPosition({ HelpScreenConfig::TextPanelX, HelpScreenConfig::TextPanelY });
    GraphicsUtils::styleRectangle(
        m_panel,
        HelpScreenConfig::PanelColor,
        HelpScreenConfig::PanelOutlineColor,
        HelpScreenConfig::PanelOutlineThickness);
    GraphicsUtils::fitSpriteTo(m_backSprite, backButtonBounds());
}

void HelpScreen::configureBackground() {
    const sf::Vector2u textureSize = m_backgroundSprite.getTexture().getSize();
    const float scale = std::max(
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y));
    m_backgroundSprite.setScale({ scale, scale });
    m_backgroundSprite.setPosition({
        (WindowConfig::Width - static_cast<float>(textureSize.x) * scale) / 2.f,
        (WindowConfig::Height - static_cast<float>(textureSize.y) * scale) / 2.f
    });
}

void HelpScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (isMouseMoved(event))
        updateHover(context.mousePosition);

    if (isLeftMousePressed(event))
        handleClick(context.mousePosition);

    if (isKeyPressed(event, sf::Keyboard::Key::Escape))
        m_closeRequested = true;
}

void HelpScreen::handleClick(sf::Vector2f position) {
    if (GeometryUtils::contains(backButtonBounds(), position))
        m_closeRequested = true;
}

void HelpScreen::updateHover(sf::Vector2f position) {
    m_backHovered = GeometryUtils::contains(backButtonBounds(), position);
}

sf::FloatRect HelpScreen::backButtonBounds() const {
    return {
        { HelpScreenConfig::Padding, HelpScreenConfig::Padding },
        { HelpScreenConfig::BackButtonSize, HelpScreenConfig::BackButtonSize }
    };
}

void HelpScreen::draw(sf::RenderWindow& target) {
    drawBackground(target);
    drawPanel(target);
    drawBackButton(target);
    drawInstructions(target);
}

void HelpScreen::drawBackground(sf::RenderTarget& target) const {
    target.draw(m_backgroundSprite);
    target.draw(m_dim);
}

void HelpScreen::drawBackButton(sf::RenderTarget& target) const {
    sf::RectangleShape button(backButtonBounds().size);
    button.setPosition(backButtonBounds().position);
    GraphicsUtils::styleRectangle(
        button,
        m_backHovered ? SettingsScreenConfig::ButtonHoverColor : SettingsScreenConfig::ButtonColor,
        HelpScreenConfig::PanelOutlineColor,
        HelpScreenConfig::PanelOutlineThickness);
    target.draw(button);
    target.draw(m_backSprite);
}

void HelpScreen::drawPanel(sf::RenderTarget& target) const {
    target.draw(m_panel);
}

void HelpScreen::drawInstructions(sf::RenderTarget& target) const {
    target.draw(TextUtils::makeCenteredText(
        m_font,
        "How to Play",
        HelpScreenConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, HelpScreenConfig::TitleY },
        HelpScreenConfig::TitleColor));

    target.draw(TextUtils::makeCenteredText(
        m_font,
        "Goal: protect the command vehicle from the enemies.",
        HelpScreenConfig::SectionTextSize,
        { WindowConfig::Width / 2.f, HelpScreenConfig::GoalY },
        HelpScreenConfig::TextColor));

    drawSectionHeading(
        target,
        "Controls & Rewards",
        { HelpScreenConfig::LeftHeadingX, HelpScreenConfig::SectionHeadingY },
        HelpScreenConfig::TitleColor);
    drawMovementRow(target, { HelpScreenConfig::LeftIconX, HelpScreenConfig::LeftRowStartY });
    drawSpacebarRow(
        target,
        { HelpScreenConfig::LeftIconX,
          HelpScreenConfig::LeftRowStartY + HelpScreenConfig::LeftRowGap });
    drawIconRow(target, GameObjects::Bomb,
        "Bomb hits earn coins.",
        { HelpScreenConfig::LeftIconX,
          HelpScreenConfig::LeftRowStartY + HelpScreenConfig::LeftRowGap * 2.f },
        HelpScreenConfig::IconSize);
    drawIconRow(target, GameObjects::Coin,
        "Finish the level: coins go to your balance.",
        { HelpScreenConfig::LeftIconX,
          HelpScreenConfig::LeftRowStartY + HelpScreenConfig::LeftRowGap * 3.f },
        HelpScreenConfig::IconSize);
    drawIconRow(target, GameObjects::CommandCar,
        "Shop assets add protection or extra damage to enemies.",
        { HelpScreenConfig::LeftIconX,
          HelpScreenConfig::LeftRowStartY + HelpScreenConfig::LeftRowGap * 4.f },
        HelpScreenConfig::IconSize);

    drawSectionHeading(
        target,
        "Game Flow",
        { HelpScreenConfig::RightHeadingX, HelpScreenConfig::GameFlowHeadingY },
        HelpScreenConfig::TitleColor);
    drawOutcomeCard(target,
        "Pause",
        { "Esc or the", "pause button." },
        { { HelpScreenConfig::RightCardX, HelpScreenConfig::PauseCardY },
          { HelpScreenConfig::RightCardWidth, HelpScreenConfig::OutcomeCardHeight } },
        HelpScreenConfig::AccentColor);
    drawOutcomeCard(target,
        "Reset",
        { "Settings ->", "Reset User Data." },
        { { HelpScreenConfig::RightCardX, HelpScreenConfig::ResetCardY },
          { HelpScreenConfig::RightCardWidth, HelpScreenConfig::OutcomeCardHeight } },
        HelpScreenConfig::AccentColor);

    drawSectionHeading(
        target,
        "Outcome",
        { HelpScreenConfig::RightHeadingX, HelpScreenConfig::OutcomeHeadingY },
        HelpScreenConfig::TitleColor);
    drawOutcomeCard(target,
        "Win",
        { "Destroy", "all enemies." },
        { { HelpScreenConfig::RightCardX, HelpScreenConfig::WinCardY },
          { HelpScreenConfig::RightCardWidth, HelpScreenConfig::OutcomeCardHeight } },
        HelpScreenConfig::AccentColor);
    drawLoseConditions(target, {
        { HelpScreenConfig::RightCardX, HelpScreenConfig::LoseCardY },
        { HelpScreenConfig::RightCardWidth, HelpScreenConfig::LoseCardHeight }
    });
}

void HelpScreen::drawIconRow(sf::RenderTarget& target,
                             GameObjects icon,
                             const std::string& text,
                             sf::Vector2f position,
                             float iconSize) const {
    sf::Sprite sprite(ResourceManager::instance().getImage(icon));
    showFirstFrame(sprite, icon);
    GraphicsUtils::fitSpriteTo(sprite, {
        { position.x + (HelpScreenConfig::LeftIconSlotWidth - iconSize) / 2.f, position.y },
        { iconSize, iconSize }
    });
    target.draw(sprite);
    TextUtils::drawMultiline(
        target,
        m_font,
        TextUtils::wrapTextLines(
            m_font,
            text,
            HelpScreenConfig::BodyTextSize,
            HelpScreenConfig::LeftTextWidth),
        HelpScreenConfig::BodyTextSize,
        { HelpScreenConfig::LeftTextX, position.y + HelpScreenConfig::LeftTextOffsetY },
        HelpScreenConfig::TextColor,
        HelpScreenConfig::HelpTextLineSpacing);
}

void HelpScreen::drawMovementRow(sf::RenderTarget& target, sf::Vector2f position) const {
    sf::Sprite left(ResourceManager::instance().getImage(GameObjects::ArrowLeft));
    sf::Sprite right(ResourceManager::instance().getImage(GameObjects::ArrowRight));
    GraphicsUtils::fitSpriteTo(left, { position, { HelpScreenConfig::IconSize, HelpScreenConfig::IconSize } });
    GraphicsUtils::fitSpriteTo(right, {
        { position.x + HelpScreenConfig::IconSize + HelpScreenConfig::IconGap, position.y },
        { HelpScreenConfig::IconSize, HelpScreenConfig::IconSize }
    });
    target.draw(left);
    target.draw(right);
    TextUtils::drawMultiline(
        target,
        m_font,
        TextUtils::wrapTextLines(
            m_font,
            "Move drone with left and right arrows.",
            HelpScreenConfig::BodyTextSize,
            HelpScreenConfig::LeftTextWidth),
        HelpScreenConfig::BodyTextSize,
        { HelpScreenConfig::LeftTextX, position.y + HelpScreenConfig::LeftTextOffsetY },
        HelpScreenConfig::TextColor,
        HelpScreenConfig::HelpTextLineSpacing);
}

void HelpScreen::drawSpacebarRow(sf::RenderTarget& target, sf::Vector2f position) const {
    sf::Sprite space(ResourceManager::instance().getImage(GameObjects::Spacebar));
    GraphicsUtils::fitSpriteTo(space, {
        position,
        { HelpScreenConfig::IconSize * 2.f + HelpScreenConfig::SpacebarExtraWidth,
          HelpScreenConfig::IconSize }
    });
    target.draw(space);
    TextUtils::drawMultiline(
        target,
        m_font,
        TextUtils::wrapTextLines(
            m_font,
            "Drop bombs with space bar.",
            HelpScreenConfig::BodyTextSize,
            HelpScreenConfig::LeftTextWidth),
        HelpScreenConfig::BodyTextSize,
        { HelpScreenConfig::LeftTextX, position.y + HelpScreenConfig::LeftTextOffsetY },
        HelpScreenConfig::TextColor,
        HelpScreenConfig::HelpTextLineSpacing);
}

void HelpScreen::drawSectionHeading(sf::RenderTarget& target,
                                    const std::string& title,
                                    sf::Vector2f position,
                                    sf::Color color) const {
    target.draw(TextUtils::makeCenteredText(
        m_font,
        title,
        HelpScreenConfig::SectionTextSize,
        position,
        color));
}

void HelpScreen::drawOutcomeCard(sf::RenderTarget& target,
                                 const std::string& title,
                                 const std::vector<std::string>& bodyLines,
                                 sf::FloatRect bounds,
                                 sf::Color titleColor) const {
    sf::RectangleShape card(bounds.size);
    card.setPosition(bounds.position);
    GraphicsUtils::styleRectangle(
        card,
        HelpScreenConfig::OutcomeCardColor,
        HelpScreenConfig::PanelOutlineColor,
        HelpScreenConfig::CardOutlineThickness);
    target.draw(card);

    target.draw(TextUtils::makeText(
        m_font,
        title,
        HelpScreenConfig::BodyTextSize,
        { bounds.position.x + HelpScreenConfig::RightTitleOffsetX,
          bounds.position.y + HelpScreenConfig::RightTitleOffsetY },
        titleColor));
    TextUtils::drawMultiline(
        target,
        m_font,
        bodyLines,
        HelpScreenConfig::BodyTextSize,
        { bounds.position.x + HelpScreenConfig::RightBodyOffsetX,
          bounds.position.y + HelpScreenConfig::RightBodyOffsetY },
        HelpScreenConfig::TextColor,
        HelpScreenConfig::RightBodyLineSpacing);
}

void HelpScreen::drawLoseConditions(sf::RenderTarget& target, sf::FloatRect bounds) const {
    sf::RectangleShape card(bounds.size);
    card.setPosition(bounds.position);
    GraphicsUtils::styleRectangle(
        card,
        HelpScreenConfig::LoseCardColor,
        HelpScreenConfig::PanelOutlineColor,
        HelpScreenConfig::CardOutlineThickness);
    target.draw(card);

    target.draw(TextUtils::makeText(
        m_font,
        "Lose Conditions",
        HelpScreenConfig::BodyTextSize,
        { bounds.position.x + HelpScreenConfig::RightTitleOffsetX,
          bounds.position.y + HelpScreenConfig::LoseHeadingOffsetY },
        HelpScreenConfig::DangerColor));

    const std::array<std::string, 3> conditions = {
        "- vehicle destroyed",
        "- no bombs + enemies",
        "- battery dead"
    };

    for (std::size_t i = 0; i < conditions.size(); ++i) {
        target.draw(TextUtils::makeText(
            m_font,
            conditions[i],
            HelpScreenConfig::LoseConditionTextSize,
            { bounds.position.x + HelpScreenConfig::LoseTextOffsetX,
              bounds.position.y + HelpScreenConfig::LoseTextStartY +
                  static_cast<float>(i) * HelpScreenConfig::LoseTextLineSpacing },
            HelpScreenConfig::TextColor));
    }
}
