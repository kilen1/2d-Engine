#include "../../include/Audio/AudioSource.h"
#include <iostream>

namespace CmakeProject1 {

    AudioSource::AudioSource() 
        : volume(1.0f), pitch(1.0f), pan(0.0f), spatialBlend(0.0f), priority(128), 
          mute(false), bypassEffects(false), bypassListenerEffects(false), bypassReverbZones(false),
          loop(false), isPlaying(false), dopplerLevel(1.0f), spread(0.0f), velocityUpdateMode(0) {
        // Initialize audio source properties
    }

    AudioSource::~AudioSource() {
        // Clean up audio resources
        Stop();
    }

    void AudioSource::SetClip(const std::string& clipPath) {
        // Implementation to set audio clip from file path
    }

    void AudioSource::SetClip(std::shared_ptr<AudioClip> clip) {
        this->clip = clip;
    }

    std::shared_ptr<AudioClip> AudioSource::GetClip() const {
        return clip;
    }

    void AudioSource::Play() {
        isPlaying = true;
        // Implementation to play the audio clip
    }

    void AudioSource::Pause() {
        isPlaying = false;
        // Implementation to pause the audio
    }

    void AudioSource::Stop() {
        isPlaying = false;
        // Implementation to stop the audio
    }

    void AudioSource::PlayOneShot(const std::string& clipPath) {
        // Implementation to play a one-shot audio clip
    }

    void AudioSource::PlayScheduled(double time) {
        // Implementation to schedule audio playback
    }

    void AudioSource::SetScheduledStartTime(double time) {
        // Implementation to set scheduled start time
    }

    void AudioSource::SetScheduledEndTime(double time) {
        // Implementation to set scheduled end time
    }

    void AudioSource::SetVolume(float vol) {
        volume = vol;
    }

    float AudioSource::GetVolume() const {
        return volume;
    }

    void AudioSource::SetPitch(float p) {
        pitch = p;
    }

    float AudioSource::GetPitch() const {
        return pitch;
    }

    void AudioSource::SetPanStereo(float panValue) {
        pan = panValue;
    }

    float AudioSource::GetPanStereo() const {
        return pan;
    }

    void AudioSource::SetSpatialBlend(float blend) {
        this->spatialBlend = blend;
    }

    float AudioSource::GetSpatialBlend() const {
        return spatialBlend;
    }

    void AudioSource::SetPriority(int priority) {
        this->priority = priority;
    }

    int AudioSource::GetPriority() const {
        return priority;
    }

    void AudioSource::SetMute(bool mute) {
        this->mute = mute;
    }

    bool AudioSource::GetMute() const {
        return mute;
    }

    void AudioSource::SetBypassEffects(bool bypass) {
        this->bypassEffects = bypass;
    }

    bool AudioSource::GetBypassEffects() const {
        return bypassEffects;
    }

    void AudioSource::SetBypassListenerEffects(bool bypass) {
        this->bypassListenerEffects = bypass;
    }

    bool AudioSource::GetBypassListenerEffects() const {
        return bypassListenerEffects;
    }

    void AudioSource::SetBypassReverbZones(bool bypass) {
        this->bypassReverbZones = bypass;
    }

    bool AudioSource::GetBypassReverbZones() const {
        return bypassReverbZones;
    }

    void AudioSource::SetLoop(bool loop) {
        this->loop = loop;
    }

    bool AudioSource::GetLoop() const {
        return loop;
    }

    bool AudioSource::IsPlaying() const {
        return isPlaying;
    }

    bool AudioSource::GetIsVirtual() const {
        return false; // Placeholder
    }

    void AudioSource::SetDopplerLevel(float level) {
        dopplerLevel = level;
    }

    float AudioSource::GetDopplerLevel() const {
        return dopplerLevel;
    }

    void AudioSource::SetSpread(float spread) {
        this->spread = spread;
    }

    float AudioSource::GetSpread() const {
        return this->spread;
    }

    void AudioSource::SetVelocityUpdateMode(int mode) {
        velocityUpdateMode = mode;
    }

    int AudioSource::GetVelocityUpdateMode() const {
        return velocityUpdateMode;
    }

}