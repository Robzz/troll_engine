#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <map>

/* This class represents a Window. It handles the display buffers, and can be assigned
 * several callback functions to react to input events, resize events, and register the
 * render loop. */
class Window {
    friend class WindowBuilder;
    public:
        ~Window();
        // Make this window the current window
        void makeCurrent();
        // Swap the front and back buffers
        void swapBuffers();
        // Return information about the underlying OpenGL context
        std::string context_info() const;

        // Register the render loop.
        void setRenderCallback(std::function<void()> f);
        /* Register the input callback. It is a function accepting 4 integer arguments :
         * the keycode, the scancode, the action, and the modifiers (?) */
        void setInputCallback(std::function<void(Window&, int, int, int, int)> f);

        /* Set the window resize callback. It as a function accepting 2 integer arguments,
         * the new width and height of the window. */
        void setResizeCallback(std::function<void(int, int)> f);

        // Start the main loop
        void mainLoop();

        // Request that the window closes
        void close();

        // Check if the window is valid
        operator bool() const;

    private:
        Window(unsigned int width, unsigned int height, std::string const& title, bool vsync);
        GLFWwindow* m_w;
        std::function<void()> m_render;
        std::function<void(Window&, int, int, int, int)> m_input;
        std::function<void(int, int)> m_resize;

        static std::map<GLFWwindow*, Window*> window_map;
        static Window* findWindowFromGlfwHandle(GLFWwindow*);
};

// This class helps constructing Window objects.
class WindowBuilder {
    public:
        WindowBuilder();
        ~WindowBuilder();

        // Set the size of the Window
        WindowBuilder& size(unsigned int width, unsigned int height);
        // Set the Window title
        WindowBuilder& title(std::string const& title);
        // Turn vsync on or off
        WindowBuilder& vsync(bool v);

        // Construct the Window
        Window build() const;

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::string m_title;
        bool m_vsync;
};
