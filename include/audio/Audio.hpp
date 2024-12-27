#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <string>

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
    void Rewind();

    bool IsPlaying() const;

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