#include "Body.hpp"
#include <FileSystem/FileSystem.hpp>
#include <WindowManager.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem&, MathLib::WindowManager& windowManager) {
    MathLib::Renderer* renderer = windowManager.MakeRenderer("Gravity simulation", 800, 800);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    MathLib::num_t prevTime = MathLib::GetTime();
    Body bodies[] = {
        Body(Vector2(0, 1), Vector2(1, 0), 1),
        Body(Vector2(-1, 0), Vector2(0, 1), 2),
    };
    const size_t bodyCount = SizeOfArray(bodies);
    bool run = true;
    while (true) {
        const MathLib::num_t currTime = MathLib::GetTime();
        const MathLib::num_t dt = currTime - prevTime;
        prevTime = currTime;
        std::cout << 1 / dt << std::endl;
        if (run) {
            for (size_t i = 0; i < bodyCount; i++) bodies[i].Move(dt);
            for (size_t i = 0; i < bodyCount - 1; i++)
            for (size_t j = i + 1; j < bodyCount; j++) bodies[i].Update(bodies[j], dt);
            renderer->Fill(0);
            for (size_t i = 0; i < bodyCount; i++) bodies[i].Draw(*renderer, 0xff0000ff);
        }
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
        if (event.type == MathLib::Event::Type::KeyPressed && event.key == ' ' && event.pressed) run = !run;
    }
}