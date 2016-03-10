#include "window.h"

#include <iostream>
#include <sstream>
#include <utility>

namespace Engine {
    std::map<GLFWwindow*, Window*> Window::window_map = std::map<GLFWwindow*, Window*>();

    Window* Window::findWindowFromGlfwHandle(GLFWwindow* w) {
        return Window::window_map.find(w) == Window::window_map.end() ? NULL : Window::window_map[w];
    }

    Window::Window(Window&& w) :
        m_w(w.m_w),
        m_im(std::move(w.m_im)),
        m_render(w.m_render),
        m_resize(w.m_resize),
        m_width(w.m_width),
        m_height(w.m_height),
        m_trackFps(true),
        m_nFrame(0),
        m_fps(0)
    {
        w.m_w = NULL;
        m_im.invertY(false);
    }

    Window::Window(int width, int height, std::string const& title, bool vsync, bool debug) :
        m_w(),
        m_im(),
        m_render(),
        m_resize(),
        m_width(width),
        m_height(height),
        m_trackFps(true),
        m_nFrame(0),
        m_fps(0)
    {
        // Use OpenGL 3.3 core profile
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GL_TRUE : GL_FALSE);

        // Create the window and make the context current
        m_w = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if(!m_w)
            std::cerr << "Cannot create window" << std::endl;
        window_map[m_w] = this;
        makeCurrent();

        glfwSetKeyCallback(m_w, [] (GLFWwindow* w, int key, int scancode, int action, int mods) {
                                       Window* win = Window::findWindowFromGlfwHandle(w);
                                       if(win)
                                           win->m_im.keyCallback(key, scancode, action, mods); });

        if(vsync)
            glfwSwapInterval(1);
    }

    Window::~Window() {
        if(m_w) {
            window_map.erase(m_w);
            glfwDestroyWindow(m_w);
        }
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

    int Window::width() const {
        return m_width;
    }

    int Window::height() const {
        return m_height;
    }

    void Window::setRenderCallback(std::function<void()> f) {
        m_render = f;
    }

    void Window::registerKeyCallback(int key, std::function<void()> f) {
        m_im.setKeyCallback(key, f);
    }

    void Window::registerMousePosCallback(MousePositionCallback f) {
        m_im.setMousePosCallback(f);
        glfwSetCursorPosCallback(m_w, [] (GLFWwindow* w, double x, double y) {
                                       Window* win = Window::findWindowFromGlfwHandle(w);
                                       if(win)
                                           win->m_im.mousePosCallback(x, y);
                                        });
    }

    void Window::registerMouseButtonCallback(MouseButtonCallback f) {
        m_im.setMouseButtonCallback(f);
        glfwSetMouseButtonCallback(m_w, [] (GLFWwindow* w, int button, int action, int mods) {
                                       Window* win = Window::findWindowFromGlfwHandle(w);
                                       if(win)
                                           win->m_im.mouseButtonCallback(button, action, mods);
                                        });
    }

    void Window::setResizeCallback(std::function<void(int, int)> f) {
        m_resize = f;
        glfwSetWindowSizeCallback(m_w, [] (GLFWwindow* w, int width, int height)
                                       { Window* win = Window::findWindowFromGlfwHandle(w);
                                         if(win) {
                                             win->m_width = width;
                                             win->m_height = height;
                                             win->m_resize(width, height); } });
    }

    void Window::showCursor(bool show) {
        glfwSetInputMode(m_w, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    void Window::mainLoop() {
        double time = glfwGetTime();
        while (!glfwWindowShouldClose(m_w))
        {
            /* Render here */
            // TODO : raise exception if no render loop
            m_render();

            /* Swap front and back buffers */
            swapBuffers();

            /* Poll and process events */
            glfwPollEvents();
            if(m_trackFps) {
                ++m_nFrame;
                double t = glfwGetTime();
                if(t - time > 1) {
                    m_fps = static_cast<float>(m_nFrame) / (t-time);
                    m_nFrame = 0;
                    time = t;
                    std::cout << "\e[1A\e[2KFPS : " << m_fps << std::endl;
                }
            }
        }
    }

    void Window::close() {
        glfwSetWindowShouldClose(m_w, GL_TRUE);
    }

    void Window::track_fps(bool enable) {
        m_trackFps = enable;
    }

    Window::operator bool() const {
        return m_w;
    }

    WindowBuilder::WindowBuilder() :
        m_height(720),
        m_width(1280),
        m_title("Window"),
        m_vsync(true),
        m_debug()
    { }

    WindowBuilder::~WindowBuilder() {

    }

    WindowBuilder& WindowBuilder::size(int width, int height) {
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

    WindowBuilder& WindowBuilder::debug(bool dbg) {
        m_debug = dbg;
        return *this;
    }

    Window WindowBuilder::build() const {
        return Window(m_width, m_height, m_title, m_vsync, m_debug);
    }
}
