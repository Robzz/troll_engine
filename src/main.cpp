#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <fstream>

#include "window.h"
#include "program.h"

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
        std::ifstream ffs("shaders/passthrough.fs");
        VertexShader vs(fvs);
        if(!vs) {
            std::cerr << "Vertex shader compile error : " << std::endl << vs.info_log() << std::endl;
            return -1;
        }
        FragmentShader fs(ffs);
        if(!fs) {
            std::cerr << "Fragment shader compile error : " << std::endl << fs.info_log() << std::endl;
            return -1;
        }
        Program p = ProgramBuilder().attach_shader(vs)
                                    .attach_shader(fs)
                                    .link();
        if(!p) {
            std::cerr << "Program link error : " << std::endl << p.info_log() << std::endl;
            return -1;
        }

        p.use();

        /* Loop until the user closes the window */
    }

        glfwTerminate();

        return 0;
    }
