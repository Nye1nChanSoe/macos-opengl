#include <mpg123.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

// Function to get the file size (in bytes)
long getFileSize(const std::string &filePath)
{
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0)
    {
        std::cerr << "Error: Unable to get file size for " << filePath << std::endl;
        return -1;
    }
    return fileStat.st_size;
}

// Decode MP3 to PCM and return PCM data
std::vector<unsigned char> decodeMP3ToPCM(const std::string &filePath, int &channels, int &sampleRate, int &bitDepth)
{
    mpg123_handle *mh = mpg123_new(nullptr, nullptr);
    if (mpg123_open(mh, filePath.c_str()) != MPG123_OK)
    {
        std::cerr << "Error opening MP3 file: " << filePath << std::endl;
        mpg123_delete(mh);
        mpg123_exit();
        return {};
    }

    mpg123_format_none(mh);
    mpg123_format(mh, 44100, MPG123_STEREO, MPG123_ENC_SIGNED_16);

    // Get format details
    long rate;
    int encoding;
    mpg123_getformat(mh, &rate, &channels, &encoding);
    sampleRate = static_cast<int>(rate);
    bitDepth = (encoding & MPG123_ENC_16) ? 16 : 8;

    // Decode MP3 to PCM
    unsigned char *buffer;
    size_t bufferSize = mpg123_outblock(mh);
    buffer = new unsigned char[bufferSize];
    std::vector<unsigned char> pcmData;

    size_t done;
    while (mpg123_read(mh, buffer, bufferSize, &done) == MPG123_OK)
    {
        pcmData.insert(pcmData.end(), buffer, buffer + done);
    }

    delete[] buffer;
    mpg123_close(mh);
    mpg123_delete(mh);

    return pcmData;
}

void getMP3FileInfo(const std::string &filePath)
{
    // Initialize mpg123
    mpg123_init();
    mpg123_handle *mh = mpg123_new(nullptr, nullptr);

    // Open the MP3 file
    if (mpg123_open(mh, filePath.c_str()) != MPG123_OK)
    {
        std::cerr << "Error opening MP3 file: " << filePath << std::endl;
        mpg123_delete(mh);
        mpg123_exit();
        return;
    }

    // Retrieve format details
    long rate;
    int channels, encoding;
    mpg123_getformat(mh, &rate, &channels, &encoding);

    // Retrieve duration
    off_t length = mpg123_length(mh); // Total number of samples
    double duration = (length > 0) ? static_cast<double>(length) / rate : 0.0;

    // Retrieve file size
    long fileSize = getFileSize(filePath);

    // Calculate bitrate (kbps)
    double bitrate = (fileSize > 0 && duration > 0) ? (fileSize * 8) / (duration * 1000.0) : 0.0;
    bitrate = std::round(bitrate);

    // Format channels
    std::string channelMode = (channels == 1) ? "Mono" : "Stereo";

    // Decode MP3 to PCM
    int pcmChannels, pcmSampleRate, pcmBitDepth;
    std::vector<unsigned char> pcmData = decodeMP3ToPCM(filePath, pcmChannels, pcmSampleRate, pcmBitDepth);

    // Output the details
    std::cout << "{\n";
    std::cout << "  \"File Name\": \"" << filePath << "\",\n";
    std::cout << "  \"Duration (seconds)\": " << std::round(duration) << ",\n";
    std::cout << "  \"Bitrate (kbps)\": " << static_cast<int>(bitrate) << ",\n";
    std::cout << "  \"Sample Rate (Hz)\": " << rate << ",\n";
    std::cout << "  \"Channels\": \"" << channelMode << "\",\n";
    std::cout << "  \"Encoding\": \"" << ((encoding & MPG123_ENC_16) ? "16-bit" : "8-bit") << "\",\n";
    std::cout << "  \"PCM Data\": {\n";
    std::cout << "    \"Sample Rate\": " << pcmSampleRate << ",\n";
    std::cout << "    \"Channels\": " << pcmChannels << ",\n";
    std::cout << "    \"Bit Depth\": " << pcmBitDepth << ",\n";
    std::cout << "    \"Data Size (bytes)\": " << pcmData.size() << "\n";
    std::cout << "  }\n";
    std::cout << "}\n";

    // Clean up
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}

// Function to initialize OpenAL
ALCdevice *initializeOpenAL(ALCcontext *&context)
{
    // Open default device
    ALCdevice *device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Error: Could not open OpenAL device." << std::endl;
        return nullptr;
    }

    // Create a context
    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context))
    {
        std::cerr << "Error: Could not create or set OpenAL context." << std::endl;
        alcCloseDevice(device);
        return nullptr;
    }

    return device;
}

// Function to clean up OpenAL resources
void cleanupOpenAL(ALCdevice *device, ALCcontext *context)
{
    alcMakeContextCurrent(nullptr);
    if (context)
        alcDestroyContext(context);
    if (device)
        alcCloseDevice(device);
}

// Function to play PCM data
void playPCMData(const std::vector<unsigned char> &pcmData, int sampleRate, int channels, int bitDepth)
{
    // Determine OpenAL format
    ALenum format;
    if (channels == 1)
    {
        format = (bitDepth == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
    }
    else if (channels == 2)
    {
        format = (bitDepth == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
    }
    else
    {
        std::cerr << "Unsupported channel count: " << channels << std::endl;
        return;
    }

    // Generate a buffer and load PCM data
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pcmData.data(), pcmData.size(), sampleRate);

    // Generate a source and attach the buffer
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    // Play the source
    alSourcePlay(source);

    std::cout << "Playing audio..." << std::endl;

    // Wait until the audio finishes playing
    ALint state;
    do
    {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    // Cleanup OpenAL resources
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

int main()
{
    // Decode MP3 to PCM
    std::string filePath = "assets/audio/masseffect-ambient.mp3";
    int channels, sampleRate, bitDepth;
    std::vector<unsigned char> pcmData = decodeMP3ToPCM(filePath, channels, sampleRate, bitDepth);

    if (pcmData.empty())
    {
        std::cerr << "Error: Failed to decode MP3 to PCM." << std::endl;
        return -1;
    }

    // Initialize OpenAL
    ALCcontext *context = nullptr;
    ALCdevice *device = initializeOpenAL(context);
    if (!device)
    {
        return -1;
    }

    // Play the PCM data
    playPCMData(pcmData, sampleRate, channels, bitDepth);

    // Clean up OpenAL
    cleanupOpenAL(device, context);

    return 0;
}

#pragma clang diagnostic pop
