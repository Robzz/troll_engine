#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
    public:
        Window(unsigned int width, unsigned int height);
        ~Window();
        void makeCurrent();

        operator bool() const;

        GLFWwindow* m_w;
};
