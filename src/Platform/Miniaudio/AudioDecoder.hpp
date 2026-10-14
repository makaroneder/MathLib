#ifndef AudioDecoder_H
#define AudioDecoder_H
#include "AudioSource.hpp"
#include <miniaudio.h>
#include <stdint.h>

struct AudioDecoder : AudioSource {
    AudioDecoder(const char* inputPath, uint32_t channels, uint32_t sampleRate, bool loop);
    virtual ~AudioDecoder(void) override;
    [[nodiscard]] virtual size_t Read(void* output, size_t frameCount) override;

    private:
    ma_decoder decoder;
    uint32_t channels;
};

#endif