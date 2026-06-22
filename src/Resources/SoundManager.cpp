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

void SoundManager::stopBackground() {
    m_backgroundMusic.stop();
    m_backgroundDucked = false;
    m_currentBackgroundVolume = BackgroundVolume;
    m_targetBackgroundVolume = BackgroundVolume;
}

void SoundManager::disableSounds() {
    m_soundsEnabled = false;
    stopBackground();
    m_levelFinishedSound.stop();
    m_missionFailedSound.stop();
}

void SoundManager::enableSounds() {
    m_soundsEnabled = true;
}

void SoundManager::update(float deltaTime) {
    restoreBackgroundVolumeIfEffectsFinished();
    changeVolume(m_targetBackgroundVolume, deltaTime);
}

void SoundManager::playLevelFinished() {
    playForegroundEffect(m_levelFinishedSound);
}

void SoundManager::playMissionFailed() {
    playForegroundEffect(m_missionFailedSound);
}

void SoundManager::playForegroundEffect(sf::Sound& sound) {
    if (!m_soundsEnabled)
        return;

    m_backgroundDucked = true;
    m_targetBackgroundVolume = DuckedBackgroundVolume;

    sound.stop();
    sound.setPlayingOffset(sf::seconds(0.f));
    sound.play();
}

void SoundManager::playBackgroundTrack(Sounds sound) {
    if (!m_soundsEnabled)
        return;

    m_backgroundMusic.stop();
    if (!m_backgroundMusic.openFromFile(ResourceManager::instance().getSoundPath(sound)))
        throw std::runtime_error("Failed to open background sound");

    m_backgroundMusic.setLooping(true);
    m_backgroundMusic.setVolume(m_currentBackgroundVolume);
    m_backgroundMusic.play();
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

    m_backgroundMusic.setVolume(m_currentBackgroundVolume);
}

void SoundManager::restoreBackgroundVolumeIfEffectsFinished() {
    if (m_backgroundDucked &&
        m_levelFinishedSound.getStatus() == sf::Sound::Status::Stopped &&
        m_missionFailedSound.getStatus() == sf::Sound::Status::Stopped) {
        m_targetBackgroundVolume = BackgroundVolume;
        m_backgroundDucked = false;
    }
}
