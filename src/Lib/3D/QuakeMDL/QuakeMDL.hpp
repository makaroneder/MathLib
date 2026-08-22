#ifndef MathLib_3D_QuakeMDL_H
#define MathLib_3D_QuakeMDL_H
#include "../Model.hpp"
#include "QuakeMDLFrame.hpp"
#include "QuakeMDLHeader.hpp"
#include "QuakeMDLTriangle.hpp"
#include "QuakeMDLSkinType.hpp"
#include "QuakeMDLFrameType.hpp"
#include "../../Image/Image.hpp"
#include "QuakeMDLFrameGroup.hpp"
#include "QuakeMDLTextureCoordinate.hpp"
#include "../../Interfaces/Saveable.hpp"

namespace MathLib {
    struct QuakeMDL : Saveable {
        Array<Model> frames;

        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
    extern Array<uint8_t> quakeMDLPalette;
}

#endif