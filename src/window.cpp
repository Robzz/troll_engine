#include "window.h"

#include <iostream>
#include <sstream>

std::map<GLFWwindow*, Window*> Window::window_map = std::map<GLFWwindow*, Window*>();

Window* Window::findWindowFromGlfwHandle(GLFWwindow* w) {
    return Window::window_map.find(w) == Window::window_map.end() ? NULL : Window::window_map[w];
}

Window::Window(unsigned int width, unsigned int height, std::string const& title, bool vsync,
               std::function<void(Window&, int, int, int, int)> input,
               std::function<void(int, int)> resize) :
    render(),
    m_inputLoop(input),
    m_resize(resize)
{
    // Use OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create the window and make the context current
    m_w = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if(!m_w)
        std::cerr << "Cannot create window" << std::endl;
    window_map[m_w] = this;
    if(m_inputLoop) {
        glfwSetKeyCallback(m_w, [] (GLFWwindow* w, int key, int scancode, int action, int mods)
                                { Window* win = Window::findWindowFromGlfwHandle(w);
                                  if(win)
                                      win->m_inputLoop(*win, key, scancode, action, mods); });
    }
    if(m_resize) {
        glfwSetWindowSizeCallback(m_w, [] (GLFWwindow* w, int width, int height)
                                       { Window* win = Window::findWindowFromGlfwHandle(w);
                                         if(win)
                                             win->m_resize(width, height); });
    }
    makeCurrent();

    if(vsync)
        glfwSwapInterval(1);
}

Window::~Window() {
    window_map.erase(m_w);
    glfwDestroyWindow(m_w);
}

void Window::makeCurrent() {
    glfwMakeContextCurrent(m_w);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_w);
}

std::string Window::context_info() const {
    std::ostringstream ss;
    const GLubyte* vendor   = glGetString (GL_VENDOR);
    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version  = glGetString (GL_VERSION);
    const GLubyte* glsl_ver = glGetString (GL_SHADING_LANGUAGE_VERSION);
    
    ss << vendor << " : " << renderer << " (" << version << "), GLSL version " << glsl_ver;
    return ss.str();
}

void Window::setRenderLoop(std::function<void()> f) {
    render = f;
}

void Window::mainLoop() {
    while (!glfwWindowShouldClose(m_w))
    {
        /* Render here */
        // TODO : raise exception if no render loop
        render();

        /* Swap front and back buffers */
        swapBuffers();

        /* Poll and process events */
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
    m_resizeCallback()
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

WindowBuilder& WindowBuilder::inputCallback(std::function<void(Window&, int, int, int, int)> f) {
    m_keyCallback = f;
    return *this;
}

WindowBuilder& WindowBuilder::resizeCallback(std::function<void(int, int)> f) {
    m_resizeCallback = f;
    return *this;
}

Window WindowBuilder::build() const {
    return Window(m_width, m_height, m_title, m_vsync, m_keyCallback, m_resizeCallback);
}
