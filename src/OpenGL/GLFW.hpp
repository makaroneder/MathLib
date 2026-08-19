#ifndef GLFW_H
#define GLFW_H
#include "GLFWWindow.hpp"

struct GLFW {
    GLFW(void);
    ~GLFW(void);
    GLFWWindow MakeWindow(const char* title, int width, int height);
    void PollEvents(void);
};

#endif