#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <map>

#include "input.h"

namespace Engine {

/* This class represents a Window. It handles the display buffers, and can be assigned
 * several callback functions to react to input events, resize events, and register the
 * render loop. */
class Window {
    friend class WindowBuilder;
    public:
        // Move constructor
        Window(Window&& w);
        // Move assignment operator
        Window& operator=(Window&& w);
        ~Window();
        // Make this window the current window
        void makeCurrent();
        // Swap the front and back buffers
        void swapBuffers();
        // Return information about the underlying OpenGL context
        std::string context_info() const;

        int width() const;
        int height() const;

        // Register the render loop.
        void setRenderCallback(std::function<void()> f);

        // Register a callback for a given key
        void registerKeyCallback(int key, std::function<void()>);

        // Register a callback for the mouse
        void registerMousePosCallback(MousePositionCallback f);

        // Register a callback for the mouse
        void registerMouseButtonCallback(MouseButtonCallback f);

        /* Set the window resize callback. It as a function accepting 2 integer arguments,
         * the new width and height of the window. */
        void setResizeCallback(std::function<void(int, int)> f);

        // Show or hide the mouse cursor
        void showCursor(bool show = true);

        // Start the main loop
        void mainLoop();

        // Request that the window closes
        void close();

        // Enable or disable fps tracking
        void track_fps(bool enable = true);

        // Check if the window is valid
        operator bool() const;

    private:
        Window(int width, int height, std::string const& title, bool vsync, bool debug);
        // No copy or assignment
        Window(Window const& other);
        Window& operator=(Window const& other);

        GLFWwindow* m_w;
        InputManager m_im;
        std::function<void()> m_render;
        std::function<void(int, int)> m_resize;
        int m_width;
        int m_height;
        bool m_trackFps;
        int m_nFrame;
        float m_fps;

        static std::map<GLFWwindow*, Window*> window_map;
        static Window* findWindowFromGlfwHandle(GLFWwindow*);
};

// This class helps constructing Window objects.
class WindowBuilder {
    public:
        WindowBuilder();
        ~WindowBuilder();

        // Set the size of the Window
        WindowBuilder& size(int width, int height);
        // Set the Window title
        WindowBuilder& title(std::string const& title);
        // Turn vsync on or off
        WindowBuilder& vsync(bool v);

        WindowBuilder& debug(bool dbg);

        // Construct the Window
        Window build() const;

    private:
        int m_height;
        int m_width;
        std::string m_title;
        bool m_vsync;
        bool m_debug;
};

}
