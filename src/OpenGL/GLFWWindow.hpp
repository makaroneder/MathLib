#ifndef GLFWWindow_H
#define GLFWWindow_H
#include "OpenGL.hpp"

struct GLFW;
struct GLFWWindow {
    friend GLFW;
    bool IsRunning(void) const;
    void SwapBuffers(void);
    void Clear(float r, float g, float b, float a);

    private:
    GLFWWindow(const char* title, int width, int height);

    GLFWwindow* window;
};

#endif