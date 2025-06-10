#include "Video.hpp"

namespace MathLib {
    Video::Video(void) : frames() {
        EmptyBenchmark
    }
    const Frame* Video::GetValue(void) const {
        StartAndReturnFromBenchmark(frames.GetValue());
    }
    bool Video::Add(const Frame& value) {
        StartAndReturnFromBenchmark(value.IsValid() && frames.Add(value));
    }
    bool Video::Reset(void) {
        StartAndReturnFromBenchmark(frames.Reset());
    }
    size_t Video::GetSize(void) const {
        StartAndReturnFromBenchmark(frames.GetSize());
    }
    Frame Video::At(size_t index) const {
        StartAndReturnFromBenchmark(frames.At(index));
    }
    Frame& Video::At(size_t index) {
        StartAndReturnFromBenchmark(frames.At(index));
    }
    size_t Video::GetWidth(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetWidth());
    }
    size_t Video::GetHeight(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetHeight());
    }
}