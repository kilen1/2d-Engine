#pragma once

#include "../Component.h"
#include <string>
#include <memory>

namespace CmakeProject1 {

    class AudioClip; // Forward declaration

    class AudioSource : public Component {
    public:
        AudioSource();
        ~AudioSource();

        // Audio clip management
        void SetClip(const std::string& clipPath);
        void SetClip(std::shared_ptr<AudioClip> clip);
        std::shared_ptr<AudioClip> GetClip() const;

        // Playback controls
        void Play();
        void Pause();
        void Stop();
        void PlayOneShot(const std::string& clipPath);
        void PlayScheduled(double time);
        void SetScheduledStartTime(double time);
        void SetScheduledEndTime(double time);

        // Audio properties
        void SetVolume(float volume);
        float GetVolume() const;
        void SetPitch(float pitch);
        float GetPitch() const;
        void SetPanStereo(float pan);
        float GetPanStereo() const;
        void SetSpatialBlend(float blend);
        float GetSpatialBlend() const;
        void SetPriority(int priority);
        int GetPriority() const;
        void SetMute(bool mute);
        bool GetMute() const;
        void SetBypassEffects(bool bypass);
        bool GetBypassEffects() const;
        void SetBypassListenerEffects(bool bypass);
        bool GetBypassListenerEffects() const;
        void SetBypassReverbZones(bool bypass);
        bool GetBypassReverbZones() const;

        // Looping
        void SetLoop(bool loop);
        bool GetLoop() const;

        // Playback status
        bool IsPlaying() const;
        bool GetIsVirtual() const;

        // 3D audio settings
        void SetDopplerLevel(float level);
        float GetDopplerLevel() const;
        void SetSpread(float spread);
        float GetSpread() const;
        void SetVelocityUpdateMode(int mode); // 0: Auto, 1: Fixed, 2: Manual
        int GetVelocityUpdateMode() const;

    private:
        std::shared_ptr<AudioClip> clip;
        float volume = 1.0f;
        float pitch = 1.0f;
        float pan = 0.0f;
        float spatialBlend = 0.0f;
        int priority = 128;
        bool mute = false;
        bool bypassEffects = false;
        bool bypassListenerEffects = false;
        bool bypassReverbZones = false;
        bool loop = false;
        bool isPlaying = false;
        float dopplerLevel = 1.0f;
        float spread = 0.0f;
        int velocityUpdateMode = 0; // Auto
    };

} // namespace CmakeProject1