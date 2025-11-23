#ifndef MathLib_Image_Video_H
#define MathLib_Image_Video_H
#include "Frame.hpp"

namespace MathLib {
    struct Video : Collection<Frame> {
        Video(void);
        [[nodiscard]] virtual bool Add(const Frame& value) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual Frame* GetValue(void) override;
        [[nodiscard]] virtual const Frame* GetValue(void) const override;
        [[nodiscard]] size_t GetWidth(void) const;
        [[nodiscard]] size_t GetHeight(void) const;
        void Resize(size_t xMultiplier, size_t yMultiplier);
        void RotateUpsideDown(void);

        protected:
        Array<Frame> frames;
    };
}

#endif