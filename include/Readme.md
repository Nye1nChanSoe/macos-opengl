# OpenAL PCM Audio Player

Decode mp3 to PCM and play using OpenAL.

---

## PCM Data Overview

PCM (Pulse Code Modulation) represents raw, uncompressed audio data:

- **Sample Rate**: Number of samples per second (e.g., 44100 Hz).  
- **Channels**: Mono (1 channel) or Stereo (2 channels).  
- **Sample Format**: Bit-depth of each sample (e.g., 16-bit, 32-bit).

---

### PCM Data Preparation

When decoding audio to PCM:

1. **Buffer for PCM Data**:  
   Use `std::vector<unsigned char>` to store PCM data.

2. **Audio Format Information**:  
   - Number of channels (1 = mono, 2 = stereo).  
   - Sample rate (e.g., 44100 Hz).  
   - Bit depth (e.g., 16-bit signed integers).

---

## OpenAL Flow for PCM Data

### 1. Initialize OpenAL

- Create a context and open a device.

---

### 2. Generate Buffers and Sources

- **Buffers**: Store the audio data.  
- **Sources**: Play the audio.

---

### 3. Load PCM Data into a Buffer

Use `alBufferData()` to transfer PCM data into an OpenAL buffer.

---

### 4. Play the Sound

Attach the buffer to the source and call `alSourcePlay()`.

---

## Core Methods in OpenAL

---

### AL and ALC Functions

- **al**: Core OpenAL functions for managing buffers, sources, and playback.  
- **alc**: OpenAL context functions for managing devices and contexts.

---

### Initialization and Cleanup

```cpp
ALCDevice* device = alcOpenDevice(nullptr);
ALCContext* context = alcCreateContext(device, nullptr);
alcMakeContextCurrent(context);

// Cleanup
alcDestroyContext(context);
alcCloseDevice(device);
```

---

### Buffer Management

```cpp
ALuint buffer;
alGenBuffers(1, &buffer);
alBufferData(buffer, AL_FORMAT_MONO16, pcmData, dataSize, sampleRate);
alDeleteBuffers(1, &buffer);
```

---

### Source Management

```cpp
ALuint source;
alGenSources(1, &source);
alSourcei(source, AL_BUFFER, buffer);
alSourcef(source, AL_GAIN, 1.0f);
alSourcePlay(source);

// Cleanup
alDeleteSources(1, &source);
```

---

### Playback Control

```cpp
ALint state;
alGetSourcei(source, AL_SOURCE_STATE, &state);
if (state == AL_PLAYING) {
    // Audio is playing
}
```

---

## References

- [OpenAL Documentation](https://www.openal.org/documentation/)  
- [PCM Audio](https://en.wikipedia.org/wiki/Pulse-code_modulation)
