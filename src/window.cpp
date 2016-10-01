#include "window.h"
#include "utility.h"

#include <iostream>
#include <sstream>
#include <utility>

#ifdef TROLL_USE_QT5
#include <QCoreApplication>
#include <QResizeEvent>
#endif

namespace Engine {
    RenderSurface* RenderSurface::s_currentRenderSurface = nullptr;

    RenderSurface::~RenderSurface() { }

    void RenderSurface::makeCurrent() { s_currentRenderSurface = this; }
    RenderSurface* RenderSurface::current() { return s_currentRenderSurface; }
    void RenderSurface::viewPort(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    Window::~Window() { }

#ifdef TROLL_USE_GLFW
    std::map<GLFWwindow*, GLFWWindow*> GLFWWindow::window_map = std::map<GLFWwindow*, GLFWWindow*>();

    GLFWWindow* GLFWWindow::findWindowFromGlfwHandle(GLFWwindow* w) {
        return GLFWWindow::window_map.find(w) == GLFWWindow::window_map.end() ? NULL : GLFWWindow::window_map[w];
    }

    GLFWWindow::GLFWWindow(GLFWWindow&& w) :
        m_w(w.m_w),
        m_title(std::move(w.m_title)),
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

    GLFWWindow::GLFWWindow(int width, int height, std::string const& title, bool vsync, bool debug) :
        m_w(),
        m_title(title),
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

        //ogl_CheckExtensions();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(&gl_debug_cb, this);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

        glfwSetKeyCallback(m_w, keyCallback);

        if(vsync)
            glfwSwapInterval(1);
    }

    GLFWWindow::~GLFWWindow() {
        if(m_w) {
            window_map.erase(m_w);
            glfwDestroyWindow(m_w);
        }
    }

    int GLFWWindow::get_attribute(int attrib) const {
        return glfwGetWindowAttrib(m_w, attrib);
    }

    void GLFWWindow::makeCurrent() {
        glfwMakeContextCurrent(m_w);
        RenderSurface::makeCurrent();
    }

    void GLFWWindow::swapBuffers() {
        glfwSwapBuffers(m_w);
    }

    std::string GLFWWindow::context_info() const {
        std::ostringstream ss;
        const auto* vendor   = glGetString (GL_VENDOR);
        const auto* renderer = glGetString (GL_RENDERER);
        const auto* version  = glGetString (GL_VERSION);
        const auto* glsl_ver = glGetString (GL_SHADING_LANGUAGE_VERSION);

        ss << vendor << " : " << renderer << " (" << version << "), GLSL version " << glsl_ver;
        return ss.str();
    }

    int GLFWWindow::width() const {
        return m_width;
    }

    int GLFWWindow::height() const {
        return m_height;
    }

    void GLFWWindow::registerKeyCallback(int key, std::function<void()> f) {
        m_im.setKeyCallback(key, f);
    }

    void GLFWWindow::registerMousePosCallback(MousePositionCallback f) {
        m_im.setMousePosCallback(f);
        glfwSetCursorPosCallback(m_w, mousePosCallback);
    }

    void GLFWWindow::registerMouseButtonCallback(MouseButtonCallback f) {
        m_im.setMouseButtonCallback(f);
        glfwSetMouseButtonCallback(m_w, mouseButtonCallback);
    }

    void GLFWWindow::setResizeCallback(std::function<void(int, int)> f) {
        m_resize = f;
        glfwSetWindowSizeCallback(m_w, [] (GLFWwindow* w, int width, int height)
                                       { GLFWWindow* win = GLFWWindow::findWindowFromGlfwHandle(w);
                                         if(win) {
                                             win->m_width = width;
                                             win->m_height = height;
                                             win->m_resize(width, height); } });
    }

    void GLFWWindow::showCursor(bool show) {
        glfwSetInputMode(m_w, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    void GLFWWindow::close() {
        glfwSetWindowShouldClose(m_w, GL_TRUE);
    }

    void GLFWWindow::pollEvents() {
        glfwPollEvents();
    }

    bool GLFWWindow::shouldClose() const {
        return glfwWindowShouldClose(m_w);
    }

    void GLFWWindow::track_fps(bool enable) {
        m_trackFps = enable;
    }

    GLFWWindow::operator bool() const {
        return m_w;
    }

    void GLFWWindow::keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
        GLFWWindow* win = GLFWWindow::findWindowFromGlfwHandle(w);
        if(win)
            win->m_im.keyCallback(key, scancode, action, mods);
    }

    void GLFWWindow::mousePosCallback(GLFWwindow* w, double x, double y) {
        GLFWWindow* win = GLFWWindow::findWindowFromGlfwHandle(w);
        if(win)
            win->m_im.mousePosCallback(x, y);
    }

    void GLFWWindow::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
        GLFWWindow* win = GLFWWindow::findWindowFromGlfwHandle(w);
        if(win)
            win->m_im.mouseButtonCallback(button, action, mods);
    }

    // TODO: proper error message formatting
    void APIENTRY GLFWWindow::gl_debug_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                      const GLchar* message, const void* userParam) {
        //const auto* w = reinterpret_cast<const GLFWWindow*>(userParam);
        std::cout << message << std::endl;
    }
#endif // TROLL_USE_GLFW

#ifdef TROLL_USE_QT5
    Qt5Window::Qt5Window(int width, int height, std::string const& title, bool vsync, bool debug) :
        Window::Window(),
        QWindow(),
        m_glContext(),
        m_renderPending(false)
    {
        resize(width, height);
        setTitle(QString::fromStdString(title));
        setSurfaceType(QWindow::OpenGLSurface);
        create();

        auto fmt = QSurfaceFormat::defaultFormat();
        fmt.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
        fmt.setVersion(3,3);
        fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
        fmt.setSwapInterval(vsync ? 1 : 0);
        fmt.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
        fmt.setOption(QSurfaceFormat::DebugContext, true);
        m_glContext.setFormat(fmt);
        if(!m_glContext.create())
            throw std::runtime_error("Cannot create OpenGL context");
        makeCurrent();

        m_resizeFunc = [&] (int w, int h) {
            viewPort(0, 0, w, h);
            requestUpdate();
        };
    }

    Qt5Window::~Qt5Window() { }

    void Qt5Window::makeCurrent() {
        if(!m_glContext.makeCurrent(static_cast<QWindow*>(this)))
            throw std::runtime_error("Cannot make context current");
        RenderSurface::makeCurrent();
    }

    void Qt5Window::swapBuffers() {
        m_glContext.swapBuffers(this);
    }

    int Qt5Window::width() const {
        return QWindow::width();
    }

    int Qt5Window::height() const {
        return QWindow::height();
    }

    bool Qt5Window::event(QEvent *event) {
        QResizeEvent* re = nullptr;
        switch (event->type()) {
        case QEvent::UpdateRequest:
            render();
            m_renderPending = false;
            break;
        case QEvent::Resize:
            re = dynamic_cast<QResizeEvent*>(event);
            m_resizeFunc(re->size().width(), re->size().height());
            break;
        default:
            return QWindow::event(event);
        }
        return true;
    }

    void Qt5Window::exposeEvent(QExposeEvent* expEvent) {
        if(isExposed()) {
            render();
        }
    }

    void Qt5Window::requestUpdate() {
        if(!m_renderPending) {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
            m_renderPending = true;
        }
    }

    void Qt5Window::setResizeCallback(std::function<void(int,int)> f) {
        m_resizeFunc = f;
    }

    void Qt5Window::close() {
        QWindow::close();
    }

    Qt5Surface::Qt5Surface(QWidget* parent, int width, int height,
                           bool vsync, bool debug) :
        RenderSurface::RenderSurface(),
        QOpenGLWidget(parent),
        m_renderPending(false)
    {
        resize(width, height);

        auto fmt = QSurfaceFormat::defaultFormat();
        fmt.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
        fmt.setVersion(3,3);
        fmt.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
        fmt.setSwapInterval(vsync ? 1 : 0);
        fmt.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
        fmt.setOption(QSurfaceFormat::DebugContext, true);
        setFormat(fmt);

        m_resizeFunc = [&] (int w, int h) {
            viewPort(0, 0, w, h);
            requestUpdate();
        };
    }

    Qt5Surface::~Qt5Surface() { }

    void Qt5Surface::makeCurrent() {
        QOpenGLWidget::makeCurrent();
        RenderSurface::makeCurrent();
    }

    void Qt5Surface::swapBuffers() { }

    bool Qt5Surface::event(QEvent *event) {
        QResizeEvent* re = nullptr;
        switch (event->type()) {
        case QEvent::UpdateRequest:
            paintGL();
            m_renderPending = false;
            break;
        default:
            return QOpenGLWidget::event(event);
        }
        return true;
    }

    void Qt5Surface::resizeGL(int w, int h) {
        m_resizeFunc(w, h);
    }

    void Qt5Surface::requestUpdate() {
        if(!m_renderPending) {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
            m_renderPending = true;
        }
    }

    int Qt5Surface::width() const {
        return QOpenGLWidget::width();
    }

    int Qt5Surface::height() const {
        return QOpenGLWidget::height();
    }

    void Qt5Surface::setResizeCallback(std::function<void(int,int)> f) {
        m_resizeFunc = f;
    }
#endif
}
