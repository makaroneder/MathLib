#include "GLSLProgram.hpp"
#include "Matrix4x4.hpp"

size_t MakeShader(const char* str, gl::GLenum type) {
    const size_t shader = gl::glCreateShader(type);
    gl::glShaderSource(shader, 1, &str, nullptr);
    gl::glCompileShader(shader);
    int status;
    gl::glGetShaderiv(shader, (gl::GLenum)GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[512];
        gl::glGetShaderInfoLog(shader, 512, nullptr, buffer);
        buffer[511] = '\0';
        MathLib::Panic(buffer);
    }
    return shader;
}
GLSLProgram::GLSLProgram(const MathLib::Sequence<size_t>& shaders) {
    const size_t size = shaders.GetSize();
    program = gl::glCreateProgram();
    for (size_t i = 0; i < size; i++) gl::glAttachShader(program, shaders.AtUnsafe(i));
    gl::glLinkProgram(program);
    for (size_t i = 0; i < size; i++) gl::glDeleteShader(shaders.AtUnsafe(i));
    int status;
    gl::glGetProgramiv(program, (gl::GLenum)GL_LINK_STATUS, &status);
    if (!status) {
        char buffer[512];
        gl::glGetProgramInfoLog(program, 512, nullptr, buffer);
        buffer[511] = '\0';
        MathLib::Panic(buffer);
    }
    Bind();
}
GLSLProgram::~GLSLProgram(void) {
    gl::glDeleteProgram(program);
}
void GLSLProgram::Bind(void) const {
    gl::glUseProgram(program);
}
size_t GLSLProgram::GetUniformLocation(const char* name) {
    return gl::glGetUniformLocation(program, name);
}
template <>
void GLSLProgram::SetUniform(const char* name, const int& value) {
    gl::glUniform1i(GetUniformLocation(name), value); 
}
template <>
void GLSLProgram::SetUniform(const char* name, const Matrix4x4& value) {
    gl::glUniformMatrix4fv(GetUniformLocation(name), 1, true, value.data); 
}