#ifndef Model_H
#define Model_H
#include "OpenGL.hpp"
#include <Interfaces/Sequence/Collection.hpp>

struct Model {
    Model(void);
    Model(const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes);
    Model(const MathLib::Collection<uint32_t>& faces, const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes);
    Model(const Model& other) = delete;
    Model(Model&& other);
    ~Model(void);
    Model& operator=(const Model& other) = delete;
    Model& operator=(Model&& other);
    void Bind(void) const;
    void Draw(OpenGLType mode) const;

    private:
    size_t vertexCount;
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
};

#endif