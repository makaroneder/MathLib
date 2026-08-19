#include "GLFW.hpp"

GLFW::GLFW(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glbinding::Binding::initialize();
}
GLFW::~GLFW(void) {
    glfwTerminate();
}
GLFWWindow GLFW::MakeWindow(const char* title, int width, int height) {
    return GLFWWindow(title, width, height);
}
void GLFW::PollEvents(void) {
    glfwPollEvents();
}