#include "Video.hpp"

namespace MathLib {
    Video::Video(void) : frames() {
        EmptyBenchmark
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
    Frame* Video::GetValue(void) {
        StartAndReturnFromBenchmark(frames.GetValue());
    }
    const Frame* Video::GetValue(void) const {
        StartAndReturnFromBenchmark(frames.GetValue());
    }
    size_t Video::GetWidth(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetWidth());
    }
    size_t Video::GetHeight(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetHeight());
    }
}