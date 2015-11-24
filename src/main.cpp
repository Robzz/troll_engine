#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <fstream>

#include "shader.h"
#include "window.h"

void error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
}

void init_libs(int argc, char** argv) {
    // Initialize GLEW and GLFW
    if (!glfwInit()) {
        std::cerr << "Error : cannot initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }


    glfwSetErrorCallback(error_callback);
}

void key_callback(Window& window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window.close();
}

int main(int argc, char** argv) {
    init_libs(argc, argv);

    {
        WindowBuilder wb;
        Window window = wb.size(1280, 720)
                          .title("Projetlololol")
                          .build();
        if (!window) {
            std::cerr << "Error : cannot create window" << std::endl;
            glfwTerminate();
            return -1;
        }
        std::ifstream fvs("shaders/passthrough.vs");
        Shader vs(fvs, GL_VERTEX_SHADER);

        /* Loop until the user closes the window */
    }

        glfwTerminate();

        return 0;
    }
