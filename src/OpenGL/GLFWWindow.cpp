#include "GLFWWindow.hpp"
#include <Typedefs.hpp>

void OnGLFWWindowResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}
GLFWWindow::GLFWWindow(const char* title, int width, int height) : window(glfwCreateWindow(width, height, title, nullptr, nullptr)) {
    if (!window) MathLib::Panic("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, &OnGLFWWindowResize);
    glViewport(0, 0, width, height);
}
bool GLFWWindow::IsRunning(void) const {
    return !glfwWindowShouldClose(window);
}
void GLFWWindow::SwapBuffers(void) {
    glfwSwapBuffers(window);
}
void GLFWWindow::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}