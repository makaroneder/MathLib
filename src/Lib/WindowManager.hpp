#ifndef MathLib_WindowManager_H
#define MathLib_WindowManager_H
#include "Renderer.hpp"

namespace MathLib {
    struct WindowManager : Allocatable {
        [[nodiscard]] virtual Renderer* MakeRenderer(const String& title, size_t width, size_t height) = 0;
    };
}

#endif