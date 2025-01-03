#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "audio/OpenAL.hpp"
#include <string>
#include <memory>

class Audio
{
public:
    Audio();
    ~Audio();

    // Load PCM data into OpenAL buffer
    // will use MP3Decoder in implementation
    bool LoadAudio(const std::string &audioFilePath);

    bool Play();
    void Stop();
    void Pause();
    void Rewind();
    void Loop(bool enable = true);

    bool IsPlaying() const;

    const inline ALuint &GetBuffer() const { return m_Buffer; }
    const inline ALuint &GetSource() const { return m_Source; }

    static std::shared_ptr<Audio> Create();

private:
    bool InitOpenAL();
    void CleanupOpenAL();

private:
    ALCdevice *m_Device;
    ALCcontext *m_Context;

    ALuint m_Buffer;
    ALuint m_Source;
};

#endif