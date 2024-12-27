#include <iostream>
#include "audio/MP3Decoder.hpp"

int main()
{
    MP3Decoder decoder;
    if (decoder.Decode("assets/audio/space-ambient.mp3"))
    {
        const PCMData &pcmData = decoder.GetDecodedPCMData();
        const MP3Data &mp3Data = decoder.GetMP3Data();

        std::cout << mp3Data << std::endl;
        std::cout << pcmData << std::endl;
    }
    return 0;
}