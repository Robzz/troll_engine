#include <GLFW/glfw3.h>

#include <functional>
#include <string>

class Window {
    public:
        Window(unsigned int width, unsigned int height, std::string const& title, bool vsync, std::function<void(Window&, int, int, int, int)> input);
        ~Window();
        void makeCurrent();
        void swapBuffers();
        std::string context_info() const;

        void setRenderLoop(std::function<void()> f);
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
        WindowBuilder& inputLoop(std::function<void(Window&, int, int, int, int)> f);

        Window build() const;

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::string m_title;
        bool m_vsync;
        std::function<void(Window&, int, int, int, int)> m_keyCallback;
};
