#include "GLFWWindow.hpp"
#include "OpenGL.hpp"
#include <Typedefs.hpp>

void OnGLFWWindowResize(GLFWwindow* window, int width, int height) {
    ((GLFWWindow*)glfwGetWindowUserPointer(window))->Resize(width, height);
}
GLFWWindow::GLFWWindow(const char* title, int width, int height) : window(glfwCreateWindow(width, height, title, nullptr, nullptr)) {
    if (!window) MathLib::Panic("Failed to create GLFW window");
    Bind();
    glfwSetWindowUserPointer(window, this);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LESS);
    glfwSetFramebufferSizeCallback(window, &OnGLFWWindowResize);
    Resize(width, height);
}
void GLFWWindow::Bind(void) const {
    glfwMakeContextCurrent(window);
}
bool GLFWWindow::IsRunning(void) const {
    return !glfwWindowShouldClose(window);
}
float GLFWWindow::GetAspectRatio(void) const {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    return (float)width / height;
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
}