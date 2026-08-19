#include "GLFWWindow.hpp"
#include <Typedefs.hpp>

void OnGLFWWindowResize(GLFWwindow* window, int width, int height) {
    ((GLFWWindow*)glfwGetWindowUserPointer(window))->Resize(width, height);
}
GLFWWindow::GLFWWindow(const char* title, int width, int height) : window(glfwCreateWindow(width, height, title, nullptr, nullptr)) {
    if (!window) MathLib::Panic("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LESS);
    glfwSetFramebufferSizeCallback(window, &OnGLFWWindowResize);
    Resize(width, height);
}
bool GLFWWindow::IsRunning(void) const {
    return !glfwWindowShouldClose(window);
}
float GLFWWindow::GetAspectRatio(void) const {
    return aspectRatio;
}
void GLFWWindow::SwapBuffers(void) {
    glfwSwapBuffers(window);
}
void GLFWWindow::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLFWWindow::Resize(int width, int height) {
    glViewport(0, 0, width, height);
    aspectRatio = (float)width / height;
}