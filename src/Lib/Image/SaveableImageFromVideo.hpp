#ifndef MathLib_Image_SaveableFromVideo_H
#define MathLib_Image_SaveableFromVideo_H
#include "SaveableImage.hpp"
#include "SaveableVideo.hpp"

namespace MathLib {
    struct SaveableImageFromVideo : SaveableImage {
        /// @brief Creates a new image
        /// @param width Width of the image
        /// @param height Height of the image
        SaveableImageFromVideo(size_t width, size_t height, SaveableVideo* video, size_t frame);
        virtual ~SaveableImageFromVideo(void) override;
        void SwitchFrame(size_t newFrame, bool saveFrame, bool loadFrame);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        private:
        SaveableVideo* video;
        size_t frame;
    };
}

#endif