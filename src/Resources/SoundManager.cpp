#include "Resources/SoundManager.h"

#include "Enums/Sounds.h"
#include "Resources/ResourceManager.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

SoundManager& SoundManager::instance() {
    static SoundManager _instance;
    return _instance;
}

SoundManager::SoundManager()
    : m_levelFinishedBuffer(ResourceManager::instance().getSoundPath(Sounds::LevelFinished))
    , m_missionFailedBuffer(ResourceManager::instance().getSoundPath(Sounds::MissionFailed))
    , m_levelFinishedSound(m_levelFinishedBuffer)
    , m_missionFailedSound(m_missionFailedBuffer) {
}

void SoundManager::startBackground() {
    playBackgroundTrack(Sounds::Background);
}

void SoundManager::startStartBackground() {
    playBackgroundTrack(Sounds::StartBackground);
}

void SoundManager::playBackgroundTrack(Sounds sound) {
    m_backgroundMusic.stop();
    if (!m_backgroundMusic.openFromFile(ResourceManager::instance().getSoundPath(sound)))
        throw std::runtime_error("Failed to open background sound");

    m_backgroundMusic.setLooping(true);
    m_backgroundMusic.setVolume(effectiveVolume(m_currentBackgroundVolume));
    m_backgroundMusic.play();
}

void SoundManager::stopBackground() {
    stopPreview();
    m_backgroundMusic.stop();
    m_backgroundDucked = false;
    m_currentBackgroundVolume = BackgroundVolume;
    m_targetBackgroundVolume = BackgroundVolume;
}

void SoundManager::update(float deltaTime) {
    if (m_previewActive) {
        m_previewElapsed += std::max(0.f, deltaTime);
        if (m_previewElapsed >= PreviewDurationSeconds ||
            m_previewMusic.getStatus() == sf::Music::Status::Stopped) {
            stopPreview();
        }
    }

    restoreBackgroundVolumeIfEffectsFinished();
    changeVolume(m_targetBackgroundVolume, deltaTime);
}

void SoundManager::restoreBackgroundVolumeIfEffectsFinished() {
    if (m_manuallyDucked)
        return;

    if (m_backgroundDucked &&
        m_levelFinishedSound.getStatus() == sf::Sound::Status::Stopped &&
        m_missionFailedSound.getStatus() == sf::Sound::Status::Stopped) {
        m_targetBackgroundVolume = BackgroundVolume;
        m_backgroundDucked = false;
    }
}

void SoundManager::changeVolume(float targetVolume, float deltaTime) {
    const float clampedTarget = std::clamp(targetVolume, 0.f, 100.f);
    const float difference = clampedTarget - m_currentBackgroundVolume;
    const float maxChange = VolumeFadeRate * std::max(deltaTime, 0.f);

    if (std::abs(difference) <= maxChange) {
        m_currentBackgroundVolume = clampedTarget;
    }
    else if (difference > 0.f) {
        m_currentBackgroundVolume += maxChange;
    }
    else {
        m_currentBackgroundVolume -= maxChange;
    }

    m_backgroundMusic.setVolume(effectiveVolume(m_currentBackgroundVolume));
}

void SoundManager::playLevelFinished() {
    playForegroundEffect(m_levelFinishedSound);
}

void SoundManager::playMissionFailed() {
    playForegroundEffect(m_missionFailedSound);
}

void SoundManager::playForegroundEffect(sf::Sound& sound) {
    m_backgroundDucked = true;
    m_targetBackgroundVolume = DuckedBackgroundVolume;

    sound.stop();
    sound.setVolume(effectiveVolume(BackgroundVolume));
    sound.setPlayingOffset(sf::seconds(0.f));
    sound.play();
}

void SoundManager::playPreview(const std::string& path) {
    stopPreview();

    m_resumeBackgroundAfterPreview =
        m_backgroundMusic.getStatus() == sf::Music::Status::Playing;
    if (m_resumeBackgroundAfterPreview)
        m_backgroundMusic.pause();

    if (!m_previewMusic.openFromFile(path))
        throw std::runtime_error("Failed to open preview sound: " + path);

    m_previewMusic.setLooping(false);
    m_previewMusic.setVolume(effectiveVolume(BackgroundVolume));
    m_previewMusic.setPlayingOffset(sf::seconds(PreviewStartSeconds));
    m_previewElapsed = 0.f;
    m_previewActive = true;
    m_previewMusic.play();
}

void SoundManager::stopPreview() {
    if (!m_previewActive)
        return;

    m_previewMusic.stop();
    m_previewActive = false;
    m_previewElapsed = 0.f;

    if (m_resumeBackgroundAfterPreview)
        m_backgroundMusic.play();

    m_resumeBackgroundAfterPreview = false;
}

void SoundManager::reduceBackgroundVolume() {
    m_manuallyDucked = true;
    m_targetBackgroundVolume = DuckedBackgroundVolume;
}

void SoundManager::restoreBackgroundVolume() {
    m_manuallyDucked = false;
    m_targetBackgroundVolume = BackgroundVolume;
}

void SoundManager::applyUserSettings(bool muted, float maxVolume) {
    m_muted = muted;
    m_maxVolume = std::clamp(maxVolume, 0.f, BackgroundVolume);
    refreshActiveVolumes();
}

float SoundManager::effectiveVolume(float volume) const {
    if (m_muted)
        return 0.f;

    const float clampedVolume = std::clamp(volume, 0.f, BackgroundVolume);
    return clampedVolume * (m_maxVolume / BackgroundVolume);
}

void SoundManager::refreshActiveVolumes() {
    m_backgroundMusic.setVolume(effectiveVolume(m_currentBackgroundVolume));
    m_previewMusic.setVolume(effectiveVolume(BackgroundVolume));
    m_levelFinishedSound.setVolume(effectiveVolume(BackgroundVolume));
    m_missionFailedSound.setVolume(effectiveVolume(BackgroundVolume));
}
