#ifndef AudioDevice_H
#define AudioDevice_H
#include "AudioSource.hpp"
#include <miniaudio.h>
#include <stdint.h>

struct AudioDevice : MathLib::Allocatable {
    AudioDevice(AudioSource& source, uint32_t channels, uint32_t sampleRate);
    virtual ~AudioDevice(void) override;
    [[nodiscard]] bool Play(void);

    private:
    static void DataCallback(ma_device* device, void* output, const void* input, uint32_t frameCount);
    ma_device device;
};

#endif