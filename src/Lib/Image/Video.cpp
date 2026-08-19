#include "Video.hpp"
#include "../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Video::Video(void) : frames() {}
    Video::Video(const Array<Frame>& frames) : frames(frames) {}
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
    Array<num_t> Video::GetDurations(void) const {
        const size_t size = frames.GetSize();
        Array<num_t> ret = Array<num_t>(size);
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = frames.AtUnsafe(i).duration;
        return ret;
    }
    void Video::SetDurations(const Function<num_t, size_t>& durations) {
        const size_t size = frames.GetSize();
        for (size_t i = 0; i < size; i++) frames.AtUnsafe(i).duration = durations(i);
    }
    void Video::Resize(size_t xMultiplier, size_t yMultiplier) {
        for (Frame& frame : frames) frame.pixels = frame.Resize(xMultiplier, yMultiplier).pixels;
    }
    Video Video::Mirror(void) const {
        Video ret = *this;
        for (Frame& frame : ret.frames) frame.pixels = frame.Mirror().pixels;
        return ret;
    }
    Video Video::RotateUpsideDown(void) const {
        Video ret = *this;
        for (Frame& frame : ret.frames) frame.pixels = frame.RotateUpsideDown().pixels;
        return ret;
    }
    Video Video::SwapXY(void) const {
        Video ret = *this;
        for (Frame& frame : ret.frames) frame.pixels = frame.SwapXY().pixels;
        return ret;
    }
    Video Video::SwapXYAndRotateUpsideDown(void) const {
        Video ret = *this;
        for (Frame& frame : ret.frames) frame.pixels = frame.SwapXYAndRotateUpsideDown().pixels;
        return ret;
    }
    Video Video::MakeWidthOdd(void) const {
        Video ret = *this;
        for (Frame& frame : ret.frames) frame.pixels = frame.MakeWidthOdd().pixels;
        return ret;
    }
    Video Video::Sub(const Interval<size_t>& interval) const {
        return Video(CollectionToArray<Frame>(SubSequence<Frame>(frames, interval)));
    }
}