#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <map>

class Window {
    public:
        Window(unsigned int width, unsigned int height, std::string const& title, bool vsync,
               std::function<void(Window&, int, int, int, int)> input,
               std::function<void(int, int)> resize);
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
        std::function<void(int, int)> m_resize;

        static std::map<GLFWwindow*, Window*> window_map;
        static Window* findWindowFromGlfwHandle(GLFWwindow*);
};

class WindowBuilder {
    public:
        WindowBuilder();
        ~WindowBuilder();

        WindowBuilder& size(unsigned int width, unsigned int height);
        WindowBuilder& title(std::string const& title);
        WindowBuilder& vsync(bool v);
        WindowBuilder& inputCallback(std::function<void(Window&, int, int, int, int)> f);
        WindowBuilder& resizeCallback(std::function<void(int, int)> f);

        Window build() const;

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::string m_title;
        bool m_vsync;
        std::function<void(Window&, int, int, int, int)> m_keyCallback;
        std::function<void(int, int)> m_resizeCallback;
};
