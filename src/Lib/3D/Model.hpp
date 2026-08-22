#ifndef MathLib_3D_Model_H
#define MathLib_3D_Model_H
#include "Mesh.hpp"

namespace MathLib {
    struct Model;
    struct Model : Comparable<Model> {
        Array<Mesh> meshes;

        Model(void);
        Model(const Mesh& mesh);
        Model(const Array<Mesh>& meshes);
        [[nodiscard]] virtual bool Equals(const Model& other) const override;
        void Draw(Renderer& renderer, uint32_t color) const;
    };
}

#endif