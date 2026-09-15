#ifndef SDL2WindowManager_H
#define SDL2WindowManager_H
#include <WindowManager.hpp>

struct SDL2WindowManager : MathLib::WindowManager {
    SDL2WindowManager(void);
    virtual ~SDL2WindowManager(void) override;
    [[nodiscard]] virtual MathLib::Renderer* MakeRenderer(const MathLib::String& title, size_t width, size_t height) override;
};

#endif