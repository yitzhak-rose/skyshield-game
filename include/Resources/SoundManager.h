#pragma once

#include <SFML/Audio.hpp>
#include <string>

enum class Sounds;

/// @brief Singleton that owns all game audio playback.
class SoundManager {
public:
    /// @brief Gets the singleton instance of the SoundManager.
    /// @return Reference to the singleton SoundManager instance.
    static SoundManager& instance();

    /// @brief Starts or restarts the gameplay background track.
    void startBackground();

    /// @brief Starts or restarts the start-screen background track.
    void startStartBackground();

    /// @brief Stops whichever background track is currently playing.
    void stopBackground();

    /// @brief Disables all sounds and stops active playback.
    void disableSounds();

    /// @brief Enables sounds after they were disabled.
    void enableSounds();

    /// @brief Updates time-sensitive sound state such as temporary volume changes.
    void update(float deltaTime);

    /// @brief Plays the level-finished effect while temporarily lowering background volume.
    void playLevelFinished();

    /// @brief Plays the mission-failed effect while temporarily lowering background volume.
    void playMissionFailed();

private:
    SoundManager();
    ~SoundManager() = default;

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    /// @brief Opens a looping background track and starts it from the beginning.
    void playBackgroundTrack(Sounds sound);

    /// @brief Restores normal music volume if all foreground effects have finished.
    void restoreBackgroundVolumeIfEffectsFinished();

    /// @brief Moves the background volume toward a target at a steady fade rate.
    void changeVolume(float targetVolume, float deltaTime);

    /// @brief Plays a foreground effect and lowers background volume until effects finish.
    void playForegroundEffect(sf::Sound& sound);

    sf::Music m_backgroundMusic;
    sf::SoundBuffer m_levelFinishedBuffer;
    sf::SoundBuffer m_missionFailedBuffer;
    sf::Sound m_levelFinishedSound;
    sf::Sound m_missionFailedSound;

    bool m_soundsEnabled = true;
    bool m_backgroundDucked = false;
    float m_currentBackgroundVolume = BackgroundVolume;
    float m_targetBackgroundVolume = BackgroundVolume;

    static constexpr float BackgroundVolume = 100.f;
    static constexpr float DuckedBackgroundVolume = BackgroundVolume * 0.25f;
    static constexpr float VolumeFadeRate = 500.f;
};
