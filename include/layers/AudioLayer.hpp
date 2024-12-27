#ifndef AUDIO_LAYER_HPP
#define AUDIO_LAYER_HPP

#include "Layer.hpp"
#include <memory>
#include "audio/Audio.hpp"

class AudioLayer : public Layer
{
public:
    AudioLayer();
    virtual ~AudioLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    std::unique_ptr<Audio> m_Audio;
};

#endif
