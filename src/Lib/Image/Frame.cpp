#include "Frame.hpp"

namespace MathLib {
    Frame::Frame(void) : Image(0, 0), duration() {}
    Frame::Frame(size_t width, size_t height, const num_t& duration) : Image(width, height), duration(duration) {}
    Frame::Frame(const Image& image, const num_t& duration) : Image(image), duration(duration) {}
    bool Frame::IsValid(void) const {
        return duration >= 0;
    }
    num_t Frame::GetDuration(void) const {
        return duration;
    }
    bool Frame::operator==(const Frame& other) const {
        return FloatsEqual<num_t>(duration, other.duration) && Image::operator==(other);
    }
    bool Frame::operator!=(const Frame& other) const {
        return !(*this == other);
    }
}