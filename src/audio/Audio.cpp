#include "audio/Audio.hpp"
#include "audio/MP3Decoder.hpp"
#include <iostream>
#include "Logger.hpp"

#define ERROR(x)         \
    Logger::Critical(x); \
    std::runtime_error(x);

Audio::Audio()
    : m_Device(nullptr), m_Context(nullptr), m_Buffer(0), m_Source(0)
{
    if (!InitOpenAL())
    {
        ERROR("Failed to initialize OpenAL")
    }
}

Audio::~Audio()
{
    CleanupOpenAL();
}

bool Audio::InitOpenAL()
{
    m_Device = alcOpenDevice(nullptr);
    if (!m_Device)
    {
        ERROR("Failed to open default OpenAL device")
        return false;
    }

    m_Context = alcCreateContext(m_Device, nullptr);
    if (!m_Context || !alcMakeContextCurrent(m_Context))
    {
        ERROR("Failed to create or set OpenAL context")
        if (m_Context)
            alcDestroyContext(m_Context);
        alcCloseDevice(m_Device);
        return false;
    }

    Logger::Info("OpenAL Vendor: {}", std::string(reinterpret_cast<const char *>(alGetString(AL_VENDOR))));
    Logger::Info("OpenAL Renderer: {}", std::string(reinterpret_cast<const char *>(alGetString(AL_RENDERER))));
    Logger::Info("OpenAL Version: {}", std::string(reinterpret_cast<const char *>(alGetString(AL_VERSION))));

    alGenBuffers(1, &m_Buffer);
    if (alGetError() != AL_NO_ERROR)
    {
        ERROR("Failed to generate OpenAL buffer")
        return false;
    }

    alGenSources(1, &m_Source);
    if (alGetError() != AL_NO_ERROR)
    {
        ERROR("Failed to generate OpenAL source")
        alDeleteBuffers(1, &m_Buffer);
        return false;
    }

    return true;
}

void Audio::CleanupOpenAL()
{
    if (m_Source)
        alDeleteSources(1, &m_Source);

    if (m_Buffer)
        alDeleteBuffers(1, &m_Buffer);

    if (m_Context)
        alcDestroyContext(m_Context);

    if (m_Device)
        alcCloseDevice(m_Device);
}

bool Audio::LoadAudio(const std::string &audioFilePath)
{
    MP3Decoder decoder;
    if (!decoder.Decode(audioFilePath))
    {
        ERROR("Failed to decode MP3 file")
        return false;
    }

    const PCMData &pcmData = decoder.GetDecodedPCMData();
    Logger::Debug("MP3 Data:");
    std::cout << decoder.GetMP3Data() << std::endl;
    Logger::Debug("PCM Data:");
    std::cout << pcmData << std::endl;

    ALenum format = (pcmData.m_Channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    alBufferData(m_Buffer, format, pcmData.m_Data.data(), pcmData.m_Data.size(), pcmData.m_SampleRate);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to buffer PCM data." << std::endl;
        return false;
    }

    alSourcei(m_Source, AL_BUFFER, m_Buffer);
    return true;
}

bool Audio::Play()
{
    alSourcePlay(m_Source);
    if (alGetError() != AL_NO_ERROR)
    {
        ERROR("Failed to play audio")
        return false;
    }
    return true;
}

void Audio::Stop()
{
    alSourceStop(m_Source);
}

void Audio::Rewind()
{
    alSourceRewind(m_Source);
}

void Audio::Loop(bool enable)
{
    alSourcei(m_Source, AL_LOOPING, enable ? AL_TRUE : AL_FALSE);
}

bool Audio::IsPlaying() const
{
    ALint state;
    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

std::unique_ptr<Audio> Audio::Create()
{
    return std::make_unique<Audio>();
}