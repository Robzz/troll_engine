#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

class Window {
    public:
        Window(unsigned int width, unsigned int height, std::string const& title, bool vsync, std::function<void()> render, std::function<void(Window&, int, int, int, int)> input);
        ~Window();
        void makeCurrent();
        void swapBuffers();

        void mainLoop();

        void close();

        operator bool() const;

    private:
        GLFWwindow* m_w;
        std::function<void()> render;
        std::function<void(Window&, int, int, int, int)> m_inputLoop;

        static bool glew_init;
};

class WindowBuilder {
    public:
        WindowBuilder();
        ~WindowBuilder();

        WindowBuilder& size(unsigned int width, unsigned int height);
        WindowBuilder& title(std::string const& title);
        WindowBuilder& vsync(bool v);
        WindowBuilder& renderLoop(std::function<void()> f);
        WindowBuilder& inputLoop(std::function<void(Window&, int, int, int, int)> f);

        Window build() const;

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::string m_title;
        bool m_vsync;
        std::function<void(Window&, int, int, int, int)> m_keyCallback;
        std::function<void(void)> m_renderCallback;
};
