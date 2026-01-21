#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <functional>

namespace CmakeProject1 {

    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();
        
        bool initialize();
        void shutdown();
        
        void update(float deltaTime);
        
        // Загрузка звуков
        Sound* loadSound(const std::string& path, bool stream = false);
        Music* loadMusic(const std::string& path);
        void unloadSound(Sound* sound);
        void unloadMusic(Music* music);
        
        // Воспроизведение
        int playSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, 
                     bool loop = false, int channel = -1);
        void playMusic(Music* music, float volume = 1.0f, bool loop = true);
        
        void stopSound(int channel);
        void stopMusic();
        void stopAll();
        
        void pauseSound(int channel);
        void pauseMusic();
        void pauseAll();
        
        void resumeSound(int channel);
        void resumeMusic();
        void resumeAll();
        
        // Параметры
        void setVolume(float volume);
        void setMusicVolume(float volume);
        void setSoundVolume(float volume);
        
        float getVolume() const { return masterVolume; }
        float getMusicVolume() const { return musicVolume; }
        float getSoundVolume() const { return soundVolume; }
        
        void setPitch(int channel, float pitch);
        void setPan(int channel, float pan); // -1 left, 0 center, 1 right
        void setDistance(int channel, float distance);
        void setPosition(int channel, const Vector2& position);
        void setListenerPosition(const Vector2& position);
        void setListenerOrientation(float angle);
        
        // Эффекты
        void addReverb(int channel, float mix, float roomSize);
        void addEcho(int channel, float delay, float decay);
        void addChorus(int channel, float rate, float depth, float mix);
        void addFlanger(int channel, float rate, float depth, float feedback);
        void addDistortion(int channel, float gain, float threshold);
        void addLowPass(int channel, float cutoff);
        void addHighPass(int channel, float cutoff);
        void addBandPass(int channel, float cutoff, float bandwidth);
        
        void clearEffects(int channel);
        
        // 3D звук
        void enable3DAudio(bool enable);
        void set3DSettings(float dopplerFactor = 1.0f, float distanceFactor = 1.0f, 
                          float rolloffFactor = 1.0f);
        
        // Потоковое воспроизведение
        void createAudioStream(const std::string& url);
        void destroyAudioStream(int streamId);
        
        // Запись
        void startRecording(int device = -1);
        void stopRecording();
        Sound* getRecording();
        
        // Анализ
        std::vector<float> getSpectrum(int channel, int size);
        float getRMS(int channel);
        float getPeak(int channel);
        
        // События
        void setSoundFinishedCallback(int channel, std::function<void()> callback);
        void setMusicFinishedCallback(std::function<void()> callback);
        
        // Группы каналов
        int createChannelGroup(const std::string& name);
        void addToGroup(int channel, int groupId);
        void setGroupVolume(int groupId, float volume);
        void setGroupPitch(int groupId, float pitch);
        void pauseGroup(int groupId);
        void resumeGroup(int groupId);
        void stopGroup(int groupId);
        
        // DSP эффекты
        class DSPEffect {
        public:
            virtual ~DSPEffect() = default;
            virtual void process(float* buffer, int length, int channels, 
                               int sampleRate) = 0;
        };
        
        void addDSPEffect(int channel, std::shared_ptr<DSPEffect> effect);
        void removeDSPEffect(int channel, std::shared_ptr<DSPEffect> effect);
        
        // MIDI
        void loadMIDI(const std::string& path);
        void playMIDI(int tempo = 120);
        void stopMIDI();
        void setMIDIInstrument(int channel, int instrument);
        
        // Генерация звуков
        Sound* generateSineWave(float frequency, float duration, float amplitude = 0.5f);
        Sound* generateSquareWave(float frequency, float duration, float amplitude = 0.5f);
        Sound* generateSawtoothWave(float frequency, float duration, float amplitude = 0.5f);
        Sound* generateTriangleWave(float frequency, float duration, float amplitude = 0.5f);
        Sound* generateNoise(float duration, float amplitude = 0.5f);
        
        // Реверберация
        void setReverbParameters(float roomSize, float damping, float wetLevel, 
                                float dryLevel, float width, float freezeMode);
        
        // Пространственный звук
        void setSpeakerMode(int mode); // Mono, Stereo, Quad, 5.1, 7.1
        void setHeadphoneMode(bool enable);
        
        // Голосовой чат
        void startVoiceChat();
        void stopVoiceChat();
        void setVoiceChatVolume(float volume);
        void setVoiceChatMute(bool mute);
        
        // Submix
        void createSubmix(const std::string& name);
        void routeToSubmix(int channel, const std::string& submix);
        void setSubmixVolume(const std::string& submix, float volume);
        
        // Timeline
        void scheduleSound(Sound* sound, float time, float volume = 1.0f);
        void scheduleMusic(Music* music, float time, float volume = 1.0f);
        
        // Синхронизация
        void syncToTransport(float bpm);
        void setTimeSignature(int numerator, int denominator);
        
        // Эквалайзер
        void setEQ(int channel, const std::vector<float>& bands);
        
        // Компрессор
        void setCompressor(int channel, float threshold, float ratio, 
                          float attack, float release, float gain);
        
        // Лимеритер
        void setLimiter(int channel, float threshold, float release);
        
        // Автоматизация
        void automateVolume(int channel, const std::vector<std::pair<float, float>>& curve);
        void automatePan(int channel, const std::vector<std::pair<float, float>>& curve);
        void automatePitch(int channel, const std::vector<std::pair<float, float>>& curve);
        
        // Сэмплер
        class Sampler {
        public:
            void loadSample(const std::string& path, int note = 60);
            void playNote(int note, float velocity = 1.0f);
            void stopNote(int note);
            
            void setADSR(float attack, float decay, float sustain, float release);
            void setFilter(float cutoff, float resonance);
            void setLFO(float rate, float amount);
        };
        
        Sampler* createSampler();
        void destroySampler(Sampler* sampler);
        
        // Sequencer
        class Sequencer {
        public:
            void setPattern(int track, const std::vector<int>& pattern);
            void setTempo(float bpm);
            void play();
            void stop();
            void record();
        };
        
        Sequencer* createSequencer();
        void destroySequencer(Sequencer* sequencer);
        
        // Ресурсы
        void preloadSounds(const std::vector<std::string>& paths);
        void unloadUnusedSounds();
        
        // Статистика
        struct Statistics {
            int soundCount = 0;
            int musicCount = 0;
            int channelCount = 0;
            int activeChannels = 0;
            float cpuUsage = 0.0f;
            float memoryUsage = 0.0f;
        };
        
        Statistics getStatistics() const;
        
        // Сохранение/загрузка
        void saveMix(const std::string& path);
        void loadMix(const std::string& path);
        
        // Тестирование
        void runAudioTest();
        
    private:
        // SDL_mixer
        int frequency = 44100;
        uint16_t format = MIX_DEFAULT_FORMAT;
        int channels = 2;
        int chunkSize = 2048;
        
        float masterVolume = 1.0f;
        float musicVolume = 0.8f;
        float soundVolume = 0.9f;
        
        std::unordered_map<std::string, std::shared_ptr<Sound>> sounds;
        std::unordered_map<std::string, std::shared_ptr<Music>> music;
        
        std::vector<Mix_Chunk*> chunks;
        std::vector<Mix_Music*> tracks;
        
        std::unordered_map<int, std::function<void()>> soundCallbacks;
        std::function<void()> musicCallback;
        
        std::unordered_map<std::string, int> channelGroups;
        std::unordered_map<int, std::vector<int>> groupChannels;
        
        // 3D звук
        bool audio3DEnabled = false;
        Vector2 listenerPosition;
        float listenerAngle = 0.0f;
        
        // Потоковое воспроизведение
        std::unordered_map<int, std::thread> streamThreads;
        std::atomic<bool> streamRunning{false};
        
        // Запись
        bool isRecording = false;
        std::vector<int16_t> recordingBuffer;
        
        // DSP
        std::unordered_map<int, std::vector<std::shared_ptr<DSPEffect>>> dspEffects;
        
        // MIDI
        std::vector<uint8_t> midiData;
        bool midiPlaying = false;
        
        // Submix
        std::unordered_map<std::string, std::vector<int>> submixes;
        
        // Timeline
        struct ScheduledSound {
            Sound* sound;
            float time;
            float volume;
        };
        
        std::vector<ScheduledSound> scheduledSounds;
        
        // Автоматизация
        struct AutomationCurve {
            std::vector<std::pair<float, float>> points;
            float currentTime = 0.0f;
        };
        
        std::unordered_map<int, AutomationCurve> volumeAutomation;
        std::unordered_map<int, AutomationCurve> panAutomation;
        std::unordered_map<int, AutomationCurve> pitchAutomation;
        
        // Сэмплеры
        std::vector<std::unique_ptr<Sampler>> samplers;
        
        // Секвенсоры
        std::vector<std::unique_ptr<Sequencer>> sequencers;
        
        // Статистика
        Statistics stats;
        std::chrono::high_resolution_clock::time_point lastStatUpdate;
        
        // Пул каналов
        std::queue<int> freeChannels;
        std::vector<bool> channelInUse;
        
        // Потоки
        std::thread audioThread;
        std::atomic<bool> audioRunning{false};
        std::mutex audioMutex;
        std::condition_variable audioCondition;
        
        // Очередь задач
        struct AudioTask {
            enum Type {
                PlaySound,
                PlayMusic,
                StopSound,
                StopMusic,
                SetVolume,
                AddEffect,
                // ...
            };
            
            Type type;
            std::any data;
        };
        
        std::queue<AudioTask> audioTasks;
        std::mutex taskMutex;
        
        // Приватные методы
        void audioThreadFunc();
        void processAudioTasks();
        void update3DAudio();
        void updateDSPEffects();
        void updateAutomation(float deltaTime);
        void updateRecording();
        void updateMIDI();
        void updateScheduledSounds(float deltaTime);
        void updateStatistics();
        
        void applyReverb(Mix_Chunk* chunk, float mix, float roomSize);
        void applyEcho(Mix_Chunk* chunk, float delay, float decay);
        
        void calculateSpectrum(Mix_Chunk* chunk, std::vector<float>& spectrum);
        float calculateRMS(Mix_Chunk* chunk);
        float calculatePeak(Mix_Chunk* chunk);
        
        void mixAudio(float* output, const float* input, int length, 
                     int channels, float volume, float pan);
        
        void resampleAudio(const int16_t* input, int inputFrames, 
                          int16_t* output, int outputFrames, 
                          int channels, float pitch);
        
        void generateWave(float* buffer, int length, float frequency, 
                         float sampleRate, WaveType type);
        
        void loadSoundAsync(const std::string& path);
        void unloadSoundAsync(Sound* sound);
        
        void saveSoundToFile(Sound* sound, const std::string& path);
        Sound* loadSoundFromFile(const std::string& path);
        
        void optimizeAudio();
        void defragmentAudioMemory();
        
        void handleAudioError(const std::string& error);
        
        static void channelFinishedCallback(int channel);
        static void musicFinishedCallback();
        
        friend class Sound;
        friend class Music;
    };
}