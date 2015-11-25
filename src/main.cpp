#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

Program buildShaderProgram() {
    std::ifstream fvs("shaders/perspective.vs");
    std::ifstream ffs("shaders/perspective.fs");
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

VBO build_sphere_mesh() {
    VBO s;
    std::vector<glm::vec4> vec;
    
    vec.push_back(glm::vec4(-.5, -.5, 0, 1));
    vec.push_back(glm::vec4(-.5, .5, 0, 1));
    vec.push_back(glm::vec4(.5, .5, 0, 1));

    s.upload_data(vec);
    
    return s;
}

VBO build_sphere_indices() {
    VBO i;

    return i;
}

void key_callback(Window& window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        window.close();
}

int main(int argc, char** argv) {
    init_libs(argc, argv);

    {
        glm::mat4 projMatrix = glm::perspective<float>(glm::radians(45.f), 1280.f/720.f, 0.1, 100),
                  worldMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10)),
                  modelView = projMatrix * worldMatrix;
        WindowBuilder wb;
        Window window = wb.size(1280, 720)
                          .title("Projetlololol")
                          .build();
        if (!window) {
            std::cerr << "Error : cannot create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        Program p = buildShaderProgram();
        GLint projMatrixLocation = p.getUniformLocation("m_modelView");
        p.use();
        p.sendUniform(projMatrixLocation, modelView);
        window.setResizeCallback([&] (int w, int h) {
            GLV(glViewport(0, 0, w, h));
            projMatrix = glm::perspective<float>(45, (float)(w)/(float)(h), 0.1, 100);
            modelView = projMatrix * worldMatrix;
            p.use();
            p.sendUniform(projMatrixLocation, modelView);
        });
        window.setInputCallback(&key_callback);
        std::cout << window.context_info() << std::endl;
        VBO sphereVbo = build_sphere_mesh();
        VAO sphereVao;
        GLint l = p.getAttributeLocation("v_position");
        sphereVao.enableVertexAttribArray(l);
        sphereVao.vertexAttribPointer(sphereVbo, l, 4);

        window.setRenderCallback([&] () {
            p.use();
            sphereVao.bind();
            GLV(glDrawArrays(GL_TRIANGLES, 0, 3));
            VAO::unbind();
            Program::noProgram();
            check_gl_errors();
        });

        /* Loop until the user closes the window */
        window.mainLoop();
    }

        glfwTerminate();

        return 0;
    }
