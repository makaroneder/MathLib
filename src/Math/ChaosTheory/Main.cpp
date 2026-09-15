#include <DynamicalSystem/LorenzSystem.hpp>
#include <FileSystem/FileSystem.hpp>
#include <WindowManager.hpp>
#include <Math/Modulo.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem&, MathLib::WindowManager& windowManager) {
    MathLib::Renderer* renderer = windowManager.MakeRenderer("Chaos theory", 800, 800);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    const MathLib::matrix_t epsMatrix = MathLib::CreateVector<MathLib::num_t>(MathLib::eps, MathLib::eps, MathLib::eps);
    const MathLib::matrix_t startPos = MathLib::CreateVector<MathLib::num_t>(0, 1, 1.05);
    const MathLib::matrix_t params = MathLib::CreateVector<MathLib::num_t>(10, 28, 8.0 / 3);
    MathLib::LorenzSystem<MathLib::num_t> systems[] = {
        MathLib::LorenzSystem<MathLib::num_t>(startPos, 10, 28, 8.0 / 3),
        MathLib::LorenzSystem<MathLib::num_t>((startPos + epsMatrix) * 2, 11, 28, 8.0 / 3),
    };
    renderer->pointMultiplier = 10;
    const MathLib::num_t dt = 0.01;
    while (true) {
        const MathLib::matrix_t a = systems[0].Update(dt);
        const MathLib::matrix_t b = systems[1].Update(dt);
        renderer->SetPixel<MathLib::num_t>(a, 0xff0000ff);
        renderer->SetPixel<MathLib::num_t>(b, 0x00ff00ff);
        std::cout << (a - b).GetLength() << std::endl;
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        if (renderer->GetEvent().type == MathLib::Event::Type::Quit) break;
    }
}