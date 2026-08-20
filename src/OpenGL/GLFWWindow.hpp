#ifndef GLFWWindow_H
#define GLFWWindow_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct GLFW;
struct GLFWWindow {
    friend GLFW;
    void Bind(void) const;
    [[nodiscard]] bool IsRunning(void) const;
    [[nodiscard]] float GetAspectRatio(void) const;
    void SwapBuffers(void);
    void Clear(float r, float g, float b, float a);
    void Resize(int width, int height);

    private:
    GLFWWindow(const char* title, int width, int height);

    GLFWwindow* window;
};

#endif