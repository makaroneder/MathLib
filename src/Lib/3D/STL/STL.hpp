#ifndef MathLib_3D_STL_H
#define MathLib_3D_STL_H
#include "../Model.hpp"
#include "STLHeader.hpp"
#include "STLTriangle.hpp"

namespace MathLib {
    struct STL : Model, Saveable {
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif