#include "GLSLProgram.hpp"
#include "Matrix4x4.hpp"

size_t MakeShader(const char* str, OpenGLType type) {
    const size_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &str, nullptr);
    glCompileShader(shader);
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        buffer[511] = '\0';
        MathLib::Panic(buffer);
    }
    return shader;
}
GLSLProgram::GLSLProgram(const MathLib::Sequence<size_t>& shaders) {
    const size_t size = shaders.GetSize();
    program = glCreateProgram();
    for (size_t i = 0; i < size; i++) glAttachShader(program, shaders.AtUnsafe(i));
    glLinkProgram(program);
    for (size_t i = 0; i < size; i++) glDeleteShader(shaders.AtUnsafe(i));
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetProgramInfoLog(program, 512, nullptr, buffer);
        buffer[511] = '\0';
        MathLib::Panic(buffer);
    }
    Bind();
}
GLSLProgram::~GLSLProgram(void) {
    glDeleteProgram(program);
}
void GLSLProgram::Bind(void) const {
    glUseProgram(program);
}
size_t GLSLProgram::GetUniformLocation(const char* name) {
    return glGetUniformLocation(program, name);
}
template <>
void GLSLProgram::SetUniform(const char* name, const int& value) {
    glUniform1i(GetUniformLocation(name), value); 
}
template <>
void GLSLProgram::SetUniform(const char* name, const Matrix4x4& value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, true, value.data); 
}
template <>
void GLSLProgram::SetUniform(const char* name, const Vector4& value) {
    glUniform4fv(GetUniformLocation(name), 1, value.data);
}