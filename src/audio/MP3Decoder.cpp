#include "audio/Mp3Decoder.hpp"
#include <mpg123.h>
#include <stdexcept>
#include "Logger.hpp"

#define ERROR(x)         \
    Logger::Critical(x); \
    std::runtime_error(x);

MP3Decoder::MP3Decoder()
{
    if (!Init())
    {
        ERROR("Failed to initialize 'mpg123' library")
    }
}

MP3Decoder::~MP3Decoder()
{
    mpg123_exit();
}

bool MP3Decoder::Decode(const std::string &filePath)
{
    mpg123_handle *mh = mpg123_new(nullptr, nullptr);
    if (!mh)
    {
        ERROR("Failed to create mpg123 handle")
        return false;
    }

    // open mp3 file
    if (mpg123_open(mh, filePath.c_str()) != MPG123_OK)
    {
        Logger::Critical("Error opening mp3 file: {}", filePath);
        std::runtime_error("Error opening mp3 file");
        return false;
    }

    long rate;
    int channels, encoding;

    if (!(mpg123_getformat(mh, &rate, &channels, &encoding) == MPG123_OK))
    {
        ERROR("Failed to parse format")
    }

    // mp3 info
    off_t length = mpg123_length(mh);
    unsigned int duration = (length > 0) ? static_cast<unsigned int>(length / rate) : 0;
    std::size_t lastSlashPos = filePath.find_last_of("/");
    std::string fileName = lastSlashPos != std::string::npos ? filePath.substr(lastSlashPos + 1) : filePath;

    m_MP3Data = {fileName,
                 duration,
                 static_cast<int>(rate),
                 channels,
                 encoding};

    // Ensure we use 16-bit signed PCM
    if (!(encoding & MPG123_ENC_16))
    {
        ERROR("Unsupported encoding format. Expected 16-bit PCM")
        mpg123_close(mh);
        mpg123_delete(mh);
        return false;
    }

    // allocate a buffer for decoded PCM data
    unsigned char *buffer;
    std::size_t bufferSize = mpg123_outblock(mh);
    buffer = new unsigned char[bufferSize];

    // a vector to hold the final PCM data
    std::vector<unsigned char> pcmData(bufferSize);
    std::size_t done;

    while (mpg123_read(mh, buffer, bufferSize, &done) == MPG123_OK)
        pcmData.insert(pcmData.end(), buffer, buffer + done);
    delete[] buffer;

    m_PCMData = {pcmData, channels, static_cast<int>(rate), 16};

    mpg123_close(mh);
    mpg123_delete(mh);

    return true;
}

bool MP3Decoder::Init()
{
    if (mpg123_init() != MPG123_OK)
        return false;
    return true;
}