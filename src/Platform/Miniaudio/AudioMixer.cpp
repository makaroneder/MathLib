#include "AudioMixer.hpp"

AudioMixer::AudioMixer(uint32_t channels) : sources(), channels(channels) {}
AudioMixer::~AudioMixer(void) {
    for (AudioSource*& source : sources) delete source;
}
bool AudioMixer::Add(AudioSource* source) {
    return source && sources.Add(source);
}
size_t AudioMixer::Read(void* output, size_t frameCount) {
    if (sources.GetSize() == 0) return 0;
    if (sources.GetSize() == 1) return sources.AtUnsafe(0)->Read(output, frameCount);
    float* const output32 = (float*)output;
    float* const buffer = new float[frameCount * channels];
    size_t maxSize = 0;
    for (AudioSource*& source : sources) {
        const size_t size = source->Read(buffer, frameCount) * channels;
        for (size_t i = 0; i < size; i++) output32[i] += buffer[i];
        if (maxSize < size) maxSize = size;
    }
    delete [] buffer;
    return maxSize / channels;
}