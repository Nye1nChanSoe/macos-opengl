#include <mpg123.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

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

int main()
{
    std::string filePath = "assets/audio/space-ambient.mp3";
    getMP3FileInfo(filePath);
    return 0;
}
