#include "layers/AudioLayer.hpp"

AudioLayer::AudioLayer()
    : Layer("AudioLayer")
{
}

AudioLayer::~AudioLayer()
{
}

void AudioLayer::OnAttach()
{
    m_Audio = Audio::Create();
    m_Audio->LoadAudio("assets/audio/space-ambient.mp3");
    m_Audio->Loop();
    m_Audio->Play();
}

void AudioLayer::OnDetach()
{
}

void AudioLayer::OnUpdate(float deltaTime)
{
}

void AudioLayer::OnRender()
{
}
