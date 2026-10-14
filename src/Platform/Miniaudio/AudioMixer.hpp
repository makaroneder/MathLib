#ifndef AudioMixer_H
#define AudioMixer_H
#include "AudioSource.hpp"
#include <Typedefs.hpp>

struct AudioMixer : AudioSource {
    AudioMixer(uint32_t channels);
    virtual ~AudioMixer(void) override;
    [[nodiscard]] bool Add(AudioSource* source);
    [[nodiscard]] virtual size_t Read(void* output, size_t frameCount) override;

    private:
    MathLib::Array<AudioSource*> sources;
    uint32_t channels;
};

#endif