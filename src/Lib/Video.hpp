#ifndef MathLib_Video_H
#define MathLib_Video_H
#include "Frame.hpp"

namespace MathLib {
    template <typename T>
    struct Video {
        Video(void) : frames() {
            EmptyBenchmark
        }
        [[nodiscard]] bool AddFrame(const Frame<T>& frame) {
            StartAndReturnFromBenchmark(frame.IsValid() && frames.Add(frame));
        }

        protected:
        Array<Frame<T>> frames;
    };
}

#endif