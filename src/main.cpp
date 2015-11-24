#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#include "gl_core_3_3.h"
#include "window.h"
#include "program.h"
#include "vbo.h"
#include "vao.h"
#include "debug.h"

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

Window buildWindow() {
    WindowBuilder wb;
    Window window = wb.size(1280, 720)
                      .title("Projetlololol")
                      .build();
    if (!window) {
        std::cerr << "Error : cannot create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return window;
}

Program buildShaderProgram() {
    std::ifstream fvs("shaders/passthrough.vs");
    std::ifstream ffs("shaders/passthrough.fs");
    VertexShader vs(fvs);
    if(!vs) {
        std::cerr << "Vertex shader compile error : " << std::endl << vs.info_log() << std::endl;
        exit(EXIT_FAILURE);
    }
    FragmentShader fs(ffs);
    if(!fs) {
        std::cerr << "Fragment shader compile error : " << std::endl << fs.info_log() << std::endl;
        exit(EXIT_FAILURE);
    }
    Program p = ProgramBuilder().attach_shader(vs)
                                .attach_shader(fs)
                                .link();
    if(!p) {
        std::cerr << "Program link error : " << std::endl << p.info_log() << std::endl;
        exit(EXIT_FAILURE);
    }
    return p;
}

VBO build_sphere() {
    VBO s;
    std::vector<glm::vec4> vec;
    
    vec.push_back(glm::vec4(-0.5, -0.5, 0, 1));
    vec.push_back(glm::vec4(-0.5, 0.5, 0, 1));
    vec.push_back(glm::vec4(0.5, 0.5, 0, 1));

    s.upload_data(vec);
    
    return s;
}

void key_callback(Window& window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window.close();
}

int main(int argc, char** argv) {
    init_libs(argc, argv);

    {
        Window window = buildWindow();
        std::cout << window.context_info() << std::endl;
        Program p = buildShaderProgram();
        VBO sphereVbo = build_sphere();
        VAO sphereVao;
        GLint l = p.getAttributeLocation("v_pos");
        sphereVao.enableVertexAttribArray(l);
        sphereVao.vertexAttribPointer(sphereVbo, l, 4);

        window.setRenderLoop([&] () {
            p.use();
            sphereVao.bind();
            GLV(glDrawArrays(GL_TRIANGLES, 0, 3));
            VAO::unbind();
            Program::no_program();
            check_gl_errors();
        });

        /* Loop until the user closes the window */
        window.mainLoop();
    }

        glfwTerminate();

        return 0;
    }
