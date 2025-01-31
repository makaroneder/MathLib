#ifndef SDL2_H
#define SDL2_H
#include "SDL2Renderer.hpp"

namespace MathLib {
    struct SDL2 : Allocatable {
        SDL2(void);
        virtual ~SDL2(void) override;
        SDL2Renderer MakeRenderer(const String& title, size_t width, size_t height);
    };
}

#endif