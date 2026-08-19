#include "Model.hpp"
#include "OpenGL.hpp"
#include <Interfaces/Sequence/EmptySequence.hpp>

template <typename T>
void GenerateBuffer(uint32_t& buffer, gl::GLenum type, const MathLib::Collection<T>& data) {
    gl::glGenBuffers(1, &buffer);
    gl::glBindBuffer(type, buffer);
    gl::glBufferData(type, data.GetSize() * sizeof(T), data.GetValue(), (gl::GLenum)GL_STATIC_DRAW);
}
Model::Model(const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes) : Model(MathLib::EmptySequence<uint32_t>(), vertices, attributes) {}
Model::Model(const MathLib::Collection<uint32_t>& faces, const MathLib::Collection<float>& vertices, const MathLib::Sequence<size_t>& attributes) {
    const size_t size = attributes.GetSize();
    const size_t verticesSize = vertices.GetSize();
    size_t stride = 0;
    for (size_t i = 0; i < size; i++) stride += attributes.AtUnsafe(i);
    gl::glGenVertexArrays(1, &vao);
    Bind();
    GenerateBuffer<float>(vbo, (gl::GLenum)GL_ARRAY_BUFFER, vertices);
    if (!faces.IsEmpty()) {
        vertexCount = faces.GetSize();
        GenerateBuffer<uint32_t>(ebo, (gl::GLenum)GL_ELEMENT_ARRAY_BUFFER, faces);
    }
    else {
        vertexCount = verticesSize / stride;
        ebo = 0;
    }
    stride *= sizeof(float);
    size_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        const size_t tmp = attributes.AtUnsafe(i);
        gl::glVertexAttribPointer(i, tmp, (gl::GLenum)GL_FLOAT, false, stride, (const void*)(sum * sizeof(float)));
        gl::glEnableVertexAttribArray(i);
        sum += tmp;
    }
}
Model::~Model(void) {
    gl::glDeleteVertexArrays(1, &vao);
    gl::glDeleteBuffers(1, &vbo);
    if (ebo) gl::glDeleteBuffers(1, &ebo);
}
void Model::Bind(void) const {
    gl::glBindVertexArray(vao);
}
void Model::Draw(void) const {
    if (ebo) glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
    else glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}