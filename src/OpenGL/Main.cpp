#include "GLFW.hpp"
#include "Model.hpp"
#include "Texture.hpp"
#include "Matrix4x4.hpp"
#include "GLSLProgram.hpp"
#include <Typedefs.hpp>
#include <ExternArray.hpp>
#include <Math/Trigonometry.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Image/Aseprite/Aseprite.hpp>
#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <iostream>

int main(int, char**) {
    try {
        GLFW glfw;
        GLFWWindow window = glfw.MakeWindow("OpenGL", 800, 800);
        MathLib::HostFileSystem fs;
        GLSLProgram program = MathLib::MakeArray<size_t>(
            MakeShader(fs.Open("src/OpenGL/Vertex.glsl"_M, MathLib::OpenMode::Read).ReadUntil('\0').GetValue(), (gl::GLenum)GL_VERTEX_SHADER),
            MakeShader(fs.Open("src/OpenGL/Fragment.glsl"_M, MathLib::OpenMode::Read).ReadUntil('\0').GetValue(), (gl::GLenum)GL_FRAGMENT_SHADER)
        );
        const float vertices[] = {
            -0.5, -0.5, -0.5,   0, 1,
            0.5, -0.5, -0.5,    1, 1,
            0.5, 0.5, -0.5,     1, 0,
            -0.5, 0.5, -0.5,    0, 0,
            -0.5, -0.5, 0.5,    0, 1,
            0.5, -0.5, 0.5,     1, 1,
            0.5, 0.5, 0.5,      1, 0,
            -0.5, 0.5, 0.5,     0, 0,
            -0.5, 0.5, 0.5,     1, 1,
            -0.5, 0.5, -0.5,    1, 0,
            -0.5, -0.5, -0.5,   0, 0,
            0.5, 0.5, 0.5,      1, 1,
            0.5, -0.5, -0.5,    0, 0,
            0.5, -0.5, 0.5,     0, 1,
            0.5, -0.5, -0.5,    1, 0,
            -0.5, 0.5, 0.5,     0, 1,
        };
        const uint32_t faces[] = {
            0, 1, 2,    2, 3, 0,
            4, 5, 6,    6, 7, 4,
            8, 9, 10,   10, 4, 8,
            11, 2, 12,  12, 13, 11,
            10, 14, 5,  5, 4, 10,
            3, 2, 11,   11, 15, 3,
        };
        Model model = Model(
            MathLib::ExternArray<uint32_t>((uint32_t*)faces, SizeOfArray(faces)),
            MathLib::ExternArray<float>((float*)vertices, SizeOfArray(vertices)),
            MathLib::VariadicSequence<size_t, 3, 2>()
        );
        MathLib::Aseprite face;
        if (!face.LoadFromPath(fs, "src/TestPrograms/OpenGL/Face.aseprite"_M)) MathLib::Panic("Failed to load face image");
        Texture texture = Texture(face.At(0), 0);
        while (window.IsRunning()) {
            const float time = glfwGetTime();
            const Matrix4x4 transform = Transalate(MathLib::Sin(time), MathLib::Cos<float>(time), -0) * Rotate(time * 3, time * 2, time * 1);
            const Matrix4x4 camera = Transalate(0, 0, -5);
            const Matrix4x4 projection = MakePerspectiveMatrix(MathLib::pi * 45 / 180, window.GetAspectRatio(), 0.1, 100);
            program.SetUniform<Matrix4x4>("transform", transform);
            program.SetUniform<Matrix4x4>("camera", camera);
            program.SetUniform<Matrix4x4>("projection", projection);

            window.Clear(0.2, 0.3, 0.3, 1);
            model.Draw();
            window.SwapBuffers();
            glfw.PollEvents();
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}