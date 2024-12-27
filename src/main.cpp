#include "audio/Audio.hpp"
#include "Logger.hpp"
#include <thread>
#include <chrono>

int main()
{
    Audio audio;
    std::string audioPath = "assets/audio/space-ambient.mp3";

    if (!audio.LoadAudio(audioPath))
    {
        Logger::Critical("Failed to load audio: {}", audioPath);
        return -1;
    }

    audio.Play();

    // keep the program running while the audio plays
    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
