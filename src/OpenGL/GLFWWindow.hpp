#ifndef GLFWWindow_H
#define GLFWWindow_H
#include "OpenGL.hpp"

struct GLFW;
struct GLFWWindow {
    friend GLFW;
    [[nodiscard]] bool IsRunning(void) const;
    [[nodiscard]] float GetAspectRatio(void) const;
    void SwapBuffers(void);
    void Clear(float r, float g, float b, float a);
    void Resize(int width, int height);

    private:
    GLFWWindow(const char* title, int width, int height);

    GLFWwindow* window;
    float aspectRatio;
};

#endif