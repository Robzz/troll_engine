#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <map>

class Window {
    public:
        Window(unsigned int width, unsigned int height, std::string const& title, bool vsync);
        ~Window();
        void makeCurrent();
        void swapBuffers();
        std::string context_info() const;

        void setRenderCallback(std::function<void()> f);
        void setInputCallback(std::function<void(Window&, int, int, int, int)> f);
        void setResizeCallback(std::function<void(int, int)> f);

        void mainLoop();

        void close();

        operator bool() const;

    private:
        GLFWwindow* m_w;
        std::function<void()> m_render;
        std::function<void(Window&, int, int, int, int)> m_input;
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

        Window build() const;

    private:
        unsigned int m_height;
        unsigned int m_width;
        std::string m_title;
        bool m_vsync;
};
