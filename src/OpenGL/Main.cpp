#include "GLFW.hpp"
#include "Model.hpp"
#include "Texture.hpp"
#include "Matrix4x4.hpp"
#include "GLSLProgram.hpp"
#include <Typedefs.hpp>
#include <ExternArray.hpp>
#include <Math/Trigonometry.hpp>
#include <Libc/HostFileSystem.hpp>
#include <3D/QuakeMDL/QuakeMDL.hpp>
#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <iostream>

Model GenerateQuakeMDLModel(const MathLib::QuakeMDL& quakeMDL, size_t frame) {
    return Model(quakeMDL.faces.At(frame), quakeMDL.vertices.At(frame), MathLib::VariadicSequence<size_t, 3, 2>());
}
int main(int, char**) {
    try {
        GLFW glfw;
        GLFWWindow window = glfw.MakeWindow("OpenGL", 800, 800);
        MathLib::HostFileSystem fs;
        GLSLProgram program = MathLib::MakeArray<size_t>(
            MakeShader(fs.Open("src/OpenGL/Shaders/Default/Vertex.glsl"_M, MathLib::OpenMode::Read).ReadUntil('\0').GetValue(), GL_VERTEX_SHADER),
            MakeShader(fs.Open("src/OpenGL/Shaders/Default/Fragment.glsl"_M, MathLib::OpenMode::Read).ReadUntil('\0').GetValue(), GL_FRAGMENT_SHADER)
        );
        MathLib::quakeMDLPalette = fs.Open("src/TestPrograms/OpenGL/Palette.lmp"_M, MathLib::OpenMode::Read).ReadAll();
        MathLib::QuakeMDL quakeMDL;
        if (!quakeMDL.LoadFromPath(fs, "src/TestPrograms/OpenGL/Soldier.mdl"_M)) MathLib::Panic("Failed to load quake model");

        Model model = GenerateQuakeMDLModel(quakeMDL, 13);
        Texture texture = Texture(quakeMDL.textures.At(0), 0);

        float prevTime = glfwGetTime();
        while (window.IsRunning()) {
            const float time = glfwGetTime();
            const float deltaTime = time - prevTime;
            std::cout << 1 / deltaTime << std::endl;
            prevTime = time;

            const Matrix4x4 transform = Rotate(-MathLib::pi / 2, 0, time);
            const Matrix4x4 camera = Transalate(0, 0, -70);
            const Matrix4x4 projection = MakePerspectiveMatrix(MathLib::pi * 45 / 180, window.GetAspectRatio(), 0.1, 100);
            program.SetUniform<Matrix4x4>("transform", transform);
            program.SetUniform<Matrix4x4>("camera", camera);
            program.SetUniform<Matrix4x4>("projection", projection);

            window.Clear(0.2, 0.3, 0.3, 1);
            model.Draw(GL_TRIANGLES);
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