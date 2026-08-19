#include "GLFW.hpp"
#include "Model.hpp"
#include "Texture.hpp"
#include "Matrix4x4.hpp"
#include "GLSLProgram.hpp"
#include <Typedefs.hpp>
#include <ExternArray.hpp>
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
            -0.5, -0.5, 0,  0, 1,
            -0.5, 0.5, 0,   0, 0,
            0.5, -0.5, 0,   1, 1,
            0.5, 0.5, 0,    1, 0,
        };
        const uint32_t faces[] = {
            0, 1, 2,
            3, 1, 2,
        };
        Model model = Model(
            MathLib::ExternArray<uint32_t>((uint32_t*)faces, SizeOfArray(faces)),
            MathLib::ExternArray<float>((float*)vertices, SizeOfArray(vertices)),
            MathLib::VariadicSequence<size_t, 3, 2>()
        );
        program.SetUniform<Matrix4x4>("transform", identity4x4);
        program.SetUniform<Matrix4x4>("camera", identity4x4);
        program.SetUniform<Matrix4x4>("projection", identity4x4);

        MathLib::Aseprite face;
        if (!face.LoadFromPath(fs, "src/TestPrograms/OpenGL/Face.aseprite"_M)) MathLib::Panic("Failed to load face image");
        Texture texture = Texture(face.At(0), 0);
        while (window.IsRunning()) {
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