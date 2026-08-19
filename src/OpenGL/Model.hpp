#ifndef Model_H
#define Model_H
#include <Interfaces/Sequence/Collection.hpp>

struct Model {
    Model(const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes);
    Model(const MathLib::Collection<uint32_t>& faces, const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes);
    ~Model(void);
    void Bind(void) const;
    void Draw(void) const;

    private:
    size_t vertexCount;
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
};

#endif