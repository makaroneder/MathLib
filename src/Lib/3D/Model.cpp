#include "Model.hpp"

namespace MathLib {
    Model::Model(void) : meshes() {}
    Model::Model(const Mesh& mesh) : meshes(MakeArray<Mesh>(mesh)) {}
    Model::Model(const Array<Mesh>& meshes) : meshes(meshes) {}
    bool Model::Equals(const Model& other) const {
        return meshes == other.meshes;
    }
    void Model::Draw(Renderer& renderer, uint32_t color) const {
        for (const Mesh& mesh : meshes) mesh.Draw(renderer, color);
    }
}