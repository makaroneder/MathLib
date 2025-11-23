#include "Video.hpp"

namespace MathLib {
    Video::Video(void) : frames() {}
    bool Video::Add(const Frame& value) {
        return value.IsValid() && frames.Add(value);
    }
    bool Video::Reset(void) {
        return frames.Reset();
    }
    size_t Video::GetSize(void) const {
        return frames.GetSize();
    }
    Frame* Video::GetValue(void) {
        return frames.GetValue();
    }
    const Frame* Video::GetValue(void) const {
        return frames.GetValue();
    }
    size_t Video::GetWidth(void) const {
        return frames.At(0).GetWidth();
    }
    size_t Video::GetHeight(void) const {
        return frames.At(0).GetHeight();
    }
    void Video::Resize(size_t xMultiplier, size_t yMultiplier) {
        for (Frame& frame : frames) frame.pixels = frame.Resize(xMultiplier, yMultiplier).pixels;
    }
    void Video::RotateUpsideDown(void) {
        for (Frame& frame : frames) frame.pixels = frame.RotateUpsideDown().pixels;
    }
}