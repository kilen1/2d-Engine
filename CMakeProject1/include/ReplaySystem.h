#pragma once

namespace CmakeProject1 {

class ReplaySystem {
public:
    static ReplaySystem& getInstance();
    
    void startRecording();
    void stopRecording();
    void saveReplay(const std::string& filename);
    void loadReplay(const std::string& filename);
    void playReplay();
    void pauseReplay();
    void stopPlayback();
    bool isRecording() const;
    bool isPlaying() const;
    void setPlaybackSpeed(float speed);

private:
    ReplaySystem() = default;
    ~ReplaySystem() = default;
    ReplaySystem(const ReplaySystem&) = delete;
    ReplaySystem& operator=(const ReplaySystem&) = delete;
    
    bool recording;
    bool playing;
    float playbackSpeed;
};

} // namespace CmakeProject1