#ifdef TROLL_USE_GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif
#ifdef TROLL_USE_QT5
#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#endif

#include <glbinding/gl33core/gl.h>
#include <functional>
#include <string>
#include <map>

#include "input.h"

namespace Engine {

/**
  * \class RenderSurface
  * \brief Interface class for render surfaces. It handles the display buffers, and can be
  * assigned several callback functions to react to input events, resize events, and register the
  * render function.
  */
class RenderSurface {
    public:
        /* Destructor */
        virtual ~RenderSurface();

        /**
          * \brief Return a pointer to the current RenderSurface, or a null pointer if none.
          */
        static RenderSurface* current();

        /**
          * \brief Make this RenderSurface the current RenderSurface.
          * This function must be called by derived classes when they are made
          * current or the current() method will return the wrong RenderSurface.
          */
        virtual void makeCurrent() = 0;

        /**
          * \brief Swap the front and back buffers
          */
        virtual void swapBuffers() = 0;
        // TODO : probably get rid of this if we're leaving the mainloop to the client
        //virtual void setRenderFunction(std::function<void()> f) = 0;

        virtual int width() const = 0;
        virtual int height() const = 0;

        void viewPort(int x, int y, int width, int height);

        /**
         * @brief Set a function to be called when the surface is resized.
         * @param f Function taking the new surface width and height as parameters.
         */
        virtual void setResizeCallback(std::function<void(int,int)> f) = 0;

    private:
        static RenderSurface* s_currentRenderSurface;
};

class Window : public RenderSurface {
    public:
        virtual ~Window();
        virtual void close() = 0;
};

#ifdef TROLL_USE_GLFW
class GLFWWindow : public Window {
    friend class RenderSurfaceBuilder;
    friend class TrollEngine;
    public:
        GLFWWindow(int width, int height, std::string const& title, bool vsync, bool debug);

        GLFWWindow() = delete;
        // Move constructor
        GLFWWindow(GLFWWindow&& w);
        // Move assignment operator
        GLFWWindow& operator=(GLFWWindow&& w);

        /* No copy */
        GLFWWindow(GLFWWindow const& other) = delete;
        GLFWWindow& operator=(GLFWWindow const& other) = delete;

        ~GLFWWindow();

        // Make this window the current window
        virtual void makeCurrent() override;
        // Swap the front and back buffers
        virtual void swapBuffers() override;
        // Return information about the underlying OpenGL context
        std::string context_info() const;

        virtual int width() const override;
        virtual int height() const override;

        int get_attribute(int attrib) const;

        // Register the render loop.
        virtual void setResizeCallback(std::function<void(int,int)> f) override;

        // Register a callback for a given key
        void registerKeyCallback(int key, std::function<void()>);

        // Register a callback for the mouse
        void registerMousePosCallback(MousePositionCallback f);

        // Register a callback for the mouse
        void registerMouseButtonCallback(MouseButtonCallback f);

        // Show or hide the mouse cursor
        void showCursor(bool show = true);

        // Request that the window closes
        virtual void close() override;

        // Process the pending events
        void pollEvents();

        // Return true if the window was requested to close
        bool shouldClose() const;

        // Enable or disable fps tracking
        void track_fps(bool enable = true);

        // Check if the window is valid
        operator bool() const;

    private:
        static void APIENTRY gl_debug_cb(gl::GLenum source, gl::GLenum type,
                                         gl::GLuint id, gl::GLenum severity,
                                         gl::GLsizei length,
                                         const gl::GLchar* message,
                                         const void* userParam);
        GLFWwindow* m_w;
        std::string m_title;
        InputManager m_im;
        std::function<void()> m_render;
        std::function<void(int, int)> m_resize;
        int m_width;
        int m_height;
        bool m_trackFps;
        int m_nFrame;
        float m_fps;

        static std::map<GLFWwindow*, GLFWWindow*> window_map;
        static GLFWWindow* findWindowFromGlfwHandle(GLFWwindow*);

        static void keyCallback(GLFWwindow* w, int key, int scancode,
                int action, int mods);
        static void mousePosCallback(GLFWwindow* w, double x, double y);
        static void mouseButtonCallback(GLFWwindow* w, int button, int action, int mods);
};
#endif
#ifdef TROLL_USE_QT5
class Qt5Window : public Window, public QWindow {
    friend class RenderSurfaceBuilder;
    public:
        /* Default constructor */
        Qt5Window() = delete;
        /* Destructor */
        virtual ~Qt5Window();

        /* No copy or move*/
        Qt5Window(Qt5Window const& other) = delete;
        Qt5Window(Qt5Window&& other) = delete;
        Qt5Window& operator=(Qt5Window const& other) = delete;
        Qt5Window& operator=(Qt5Window&& other) = delete;

        virtual void makeCurrent() override;

        virtual void swapBuffers() override;

        virtual int width() const override;
        virtual int height() const override;

        bool event(QEvent *event) override;
        void exposeEvent(QExposeEvent* expEvent) override;

        void requestUpdate();
        virtual void render() = 0;

        virtual void setResizeCallback(std::function<void(int,int)> f) override;

        virtual void close() override;

    protected:
        Qt5Window(int width, int height, std::string const& title, bool vsync, bool debug);
        QOpenGLContext m_glContext;
        std::function<void(int,int)> m_resizeFunc;
        bool m_renderPending;
};

class Qt5Surface : public RenderSurface, public QOpenGLWidget {
    friend class RenderSurfaceBuilder;
    public:
        /* Default constructor */
        Qt5Surface() = delete;
        /* Destructor */
        virtual ~Qt5Surface();

        /* No copy or move*/
        Qt5Surface(Qt5Surface const& other) = delete;
        Qt5Surface(Qt5Surface&& other) = delete;
        Qt5Surface& operator=(Qt5Surface const& other) = delete;
        Qt5Surface& operator=(Qt5Surface&& other) = delete;

        virtual void makeCurrent() override;

        /**
         * @brief swapBuffers is apparently automatically called by Qt sometime after
         * paintGL returns, so this method is implemented as a no-op.
         */
        virtual void swapBuffers() override;

        virtual int width() const override;
        virtual int height() const override;

        bool event(QEvent *event) override;
        virtual void resizeGL(int w, int h) override;

        void requestUpdate();

        virtual void setResizeCallback(std::function<void(int,int)> f) override;

    protected:
        Qt5Surface(QWidget* parent, int width, int height, bool vsync, bool debug);
        std::function<void(int,int)> m_resizeFunc;
        bool m_renderPending;
};
#endif
}
