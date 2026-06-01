#ifndef MathLib_Image_Aseprite_H
#define MathLib_Image_Aseprite_H
#include "../SaveableVideo.hpp"

namespace MathLib {
    struct Aseprite : SaveableVideo {
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif