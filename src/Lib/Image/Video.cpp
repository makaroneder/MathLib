#include "Video.hpp"

namespace MathLib {
    Video::Video(void) : frames() {
        EmptyBenchmark
    }
    Frame Video::At(size_t index) const {
        StartAndReturnFromBenchmark(frames.At(index));
    }
    Frame& Video::At(size_t index) {
        StartAndReturnFromBenchmark(frames.At(index));
    }
    size_t Video::GetFrameCount(void) const {
        StartAndReturnFromBenchmark(frames.GetSize());
    }
    size_t Video::GetWidth(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetWidth());
    }
    size_t Video::GetHeight(void) const {
        StartAndReturnFromBenchmark(frames.At(0).GetHeight());
    }
    bool Video::AddFrame(const Frame& frame) {
        StartAndReturnFromBenchmark(frame.IsValid() && frames.Add(frame));
    }
}