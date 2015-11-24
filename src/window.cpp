#include "window.h"

#include <iostream>

bool Window::glew_init = false;

Window::Window(unsigned int width, unsigned int height, std::string const& title, bool vsync, std::function<void()> render, std::function<void(Window&, int, int, int, int)> input) :
    render(render),
    m_inputLoop(input)
{
    // Use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_w = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if(!m_w)
        std::cerr << "Shit flew through the window" << std::endl;
    if(m_inputLoop) {
        std::function<void(GLFWwindow*, int, int, int, int)> cb = [&] (GLFWwindow* w, int key, int scancode, int action, int mods)
                                                                  { m_inputLoop(*this, key, scancode, action, mods); } ;
        glfwSetKeyCallback(m_w, cb.target<void(GLFWwindow*, int, int, int, int)>());
    }
    makeCurrent();

    // GLEW needs a GL context to be initialized, so we do it here...
    if(!glew_init) {
        if(glewInit() != GLEW_OK) {
            std::cerr << "Error : cannot initialize GLEW" << std::endl;
            exit(EXIT_FAILURE);
        }
        glew_init = true;
    }

    if(vsync)
        glfwSwapInterval(1);
}

Window::~Window() {
    glfwDestroyWindow(m_w);
}

void Window::makeCurrent() {
    glfwMakeContextCurrent(m_w);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_w);
}

void Window::mainLoop() {
    while (!glfwWindowShouldClose(m_w))
    {
        /* Render here */
        render();

        /* Swap front and back buffers */
        swapBuffers();

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void Window::close() {
    glfwSetWindowShouldClose(m_w, GL_TRUE);
}

Window::operator bool() const {
    return m_w;
}

WindowBuilder::WindowBuilder() :
    m_height(720),
    m_width(1280),
    m_title("LMG"),
    m_vsync(true),
    m_keyCallback(),
    m_renderCallback()
{ }

WindowBuilder::~WindowBuilder() {

}

WindowBuilder& WindowBuilder::size(unsigned int width, unsigned int height) {
    m_width = width;
    m_height = height;
    return *this;
}

WindowBuilder& WindowBuilder::title(std::string const& title) {
    m_title = title;
    return *this;
}

WindowBuilder& WindowBuilder::vsync(bool v) {
    m_vsync = v;
    return *this;
}

WindowBuilder& WindowBuilder::renderLoop(std::function<void()> f) {
    m_renderCallback = f;
    return *this;
}

WindowBuilder& WindowBuilder::inputLoop(std::function<void(Window&, int, int, int, int)> f) {
    m_keyCallback = f;
    return *this;
}

Window WindowBuilder::build() const {
    return Window(m_width, m_height, m_title, m_vsync, m_renderCallback, m_keyCallback);
}
