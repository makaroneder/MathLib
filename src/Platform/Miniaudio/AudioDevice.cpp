#include "AudioDevice.hpp"
#include <Typedefs.hpp>

AudioDevice::AudioDevice(AudioSource& source, uint32_t channels, uint32_t sampleRate) {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = channels;
    config.sampleRate = sampleRate;
    config.dataCallback = &DataCallback;
    config.pUserData = &source;
    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) MathLib::Panic("Failed to init audio device");
}
AudioDevice::~AudioDevice(void) {
    ma_device_uninit(&device);
}
bool AudioDevice::Play(void) {
    return ma_device_start(&device) == MA_SUCCESS;
}
void AudioDevice::DataCallback(ma_device* device, void* output, const void* input, uint32_t frameCount) {
    (void)input;
    MathLib::MemorySet32(output, frameCount * device->playback.channels, 0);
    (void)((AudioSource*)device->pUserData)->Read(output, frameCount);
}