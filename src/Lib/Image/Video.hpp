#ifndef MathLib_Image_Video_H
#define MathLib_Image_Video_H
#include "Frame.hpp"

namespace MathLib {
    struct Video {
        Video(void);
        [[nodiscard]] Frame At(size_t index) const;
        [[nodiscard]] Frame& At(size_t index);
        [[nodiscard]] size_t GetFrameCount(void) const;
        [[nodiscard]] size_t GetWidth(void) const;
        [[nodiscard]] size_t GetHeight(void) const;
        [[nodiscard]] bool AddFrame(const Frame& frame);

        protected:
        Array<Frame> frames;
    };
}

#endif