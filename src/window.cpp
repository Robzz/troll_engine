#include "window.h"

Window::Window(unsigned int width, unsigned int height) {
    // Use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_w = glfwCreateWindow(width, height, "LMG", NULL, NULL);
}

Window::~Window() {
    glfwDestroyWindow(m_w);
}

void Window::makeCurrent() {
    glfwMakeContextCurrent(m_w);
}

Window::operator bool() const {
    return m_w;
}
