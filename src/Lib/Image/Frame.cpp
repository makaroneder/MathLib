#include "Frame.hpp"

namespace MathLib {
    Frame::Frame(void) : Image(0, 0), duration() {
        EmptyBenchmark
    }
    Frame::Frame(size_t width, size_t height, const MathLib::num_t& duration) : Image(width, height), duration(duration) {
        EmptyBenchmark
    }
    Frame::Frame(const Image& image, const MathLib::num_t& duration) : Image(image), duration(duration) {
        EmptyBenchmark
    }
    bool Frame::IsValid(void) const {
        StartAndReturnFromBenchmark(duration >= 0);
    }
    MathLib::num_t Frame::GetDuration(void) const {
        StartAndReturnFromBenchmark(duration);
    }
    bool Frame::operator==(const Frame& other) const {
        StartAndReturnFromBenchmark(FloatsEqual<MathLib::num_t>(duration, other.duration) && Image::operator==(other));
    }
    bool Frame::operator!=(const Frame& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}