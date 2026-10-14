#include "AudioDecoder.hpp"
#include <Typedefs.hpp>

AudioDecoder::AudioDecoder(const char* inputPath, uint32_t channels, uint32_t sampleRate, bool loop) : channels(channels) {
    const ma_decoder_config config = ma_decoder_config_init(ma_format_f32, channels, sampleRate);
    if (ma_decoder_init_file(inputPath, &config, &decoder) != MA_SUCCESS) MathLib::Panic("Failed to init audio decoder");
    if (ma_data_source_set_looping(&decoder, loop) != MA_SUCCESS) MathLib::Panic("Failed to set looping mode of audio decoder");
}
AudioDecoder::~AudioDecoder(void) {
    if (ma_decoder_uninit(&decoder) != MA_SUCCESS) MathLib::Panic("Failed to delete audio decoder");
}
size_t AudioDecoder::Read(void* output, size_t frameCount) {
    ma_uint64 read = 0;
    return ma_data_source_read_pcm_frames(&decoder, output, frameCount, &read) == MA_SUCCESS ? read : 0;
}