#ifndef GLSLProgram_H
#define GLSLProgram_H
#include "OpenGL.hpp"
#include <Typedefs.hpp>

struct GLSLProgram {
    GLSLProgram(const MathLib::Sequence<size_t>& shaders);
    ~GLSLProgram(void);
    void Bind(void) const;
    template <typename T>
    void SetUniform(const char* name, const T& value);

    private:
    size_t GetUniformLocation(const char* name);

    size_t program;
};
size_t MakeShader(const char* str, gl::GLenum type);

#endif