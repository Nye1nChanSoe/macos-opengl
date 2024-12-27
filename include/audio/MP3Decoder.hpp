#ifndef MP3_DECODER_HPP
#define MP3_DECODER_HPP

#include <vector>
#include <string>
#include <iostream>

class MP3Decoder
{
private:
    PCMData m_PCMData;
    MP3Data m_MP3Data;

public:
    MP3Decoder();
    ~MP3Decoder();

    bool Decode(const std::string &filePath);

    const PCMData &GetDecodedPCMData() const { return m_PCMData; }
    const MP3Data &GetMP3Data() const { return m_MP3Data; }

private:
    bool Init();
};

struct PCMData
{
    std::vector<unsigned char> m_Data;
    int m_Channels;
    int m_SampleRate;
    int m_BitDepth;

    friend std::ostream &operator<<(std::ostream &os, const PCMData &data)
    {
        os << "PCM Data Info\n";
        os << "{\n";
        os << "  \"Channels\": " << data.m_Channels << ",\n";
        os << "  \"SampleRate\": " << data.m_SampleRate << ",\n";
        os << "  \"BitDepth\": " << data.m_BitDepth << ",\n";
        os << "  \"Size(bytes)\": " << data.m_Data.size() << "\n";
        os << "}";
        return os;
    }
};

struct MP3Data
{
    std::string m_Filename;
    unsigned int m_Duration;
    int m_SampleRate;
    int m_Channels;
    int m_Encoding;

    friend std::ostream &operator<<(std::ostream &os, const MP3Data &data)
    {
        os << "MP3 Data Info\n";
        os << "{\n";
        os << "  \"FileName\": \"" << data.m_Filename << "\",\n";
        os << "  \"Duration(seconds)\": " << data.m_Duration << ",\n";
        os << "  \"SampleRate(Hz)\": " << data.m_SampleRate << ",\n";
        os << "  \"Channels\": \"" << (data.m_Channels == 1 ? "Mono" : "Stereo") << "\",\n";
        os << "  \"Encoding\": \"" << (data.m_Encoding == 16 ? "16-bit" : "8-bit") << "\"\n";
        os << "}";
        return os;
    }
};

#endif