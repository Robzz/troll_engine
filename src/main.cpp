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
    const int nMeridians = 30;
    const int nParallels = 30;
    const float radius = 0.5;
    const float dPhi = M_PI / (nParallels+1);
    const float dTheta = 2 * M_PI / (nMeridians-1);
    std::vector<glm::vec3> vec;
    vec.reserve(nMeridians*nParallels);
    
    // Generate points along the parallels
    for(int i = 0 ; i != nParallels; ++i) {
        float phi = M_PI/2 + i*dPhi;
        float y = radius * sin(phi);
        float r = radius * cos(phi);
        for(int j = 0 ; j != nMeridians ; ++j) {
            float theta = j * dTheta;
            vec.push_back(glm::vec3(r*sin(theta), y, r*cos(theta)));
            // The normal is just the normalized position, but we can let glVertexAttribPointer do that
        }
    }

    VBO s;
    s.upload_data(vec);
    return s;
}

VBO build_sphere_indices() {
    const int nMeridians = 30;
    const int nParallels = 30;
    std::vector<GLshort> vec;
    
    for(int i = 0 ; i != nParallels-1 ; ++i) {
        for(int j = 0 ; j != nMeridians ; ++j) {
            vec.push_back(nParallels * i + j);
            vec.push_back(nParallels * i + j + 1);
            vec.push_back(nParallels * (i + 1) + j + 1);
            vec.push_back(nParallels * (i + 1) + j + 1);
            vec.push_back(nParallels * (i + 1) + j);
            vec.push_back(nParallels * i + j);
        }
    }

    VBO i;
    i.upload_data(vec);
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
                  worldMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5)),
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
        VBO sphereIndices = build_sphere_indices();
        VAO sphereVao;
        GLint posIndex = p.getAttributeLocation("v_position");
        GLint normalIndex = p.getAttributeLocation("v_normal");
        sphereVao.enableVertexAttribArray(posIndex);
        sphereVao.vertexAttribPointer(sphereVbo, posIndex, 3);
        sphereVao.enableVertexAttribArray(normalIndex);
        sphereVao.vertexAttribPointer(sphereVbo, normalIndex, 3, 0, NULL, GL_FLOAT, GL_TRUE);
        VAO::unbind();

        glDisable(GL_CULL_FACE);
        // Setup depth test
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);
        glClearDepth(1.0f);
        window.setRenderCallback([&] () {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            p.use();
            sphereVao.bind();
            sphereIndices.bind(GL_ELEMENT_ARRAY_BUFFER);
            GLV(glDrawElements(GL_TRIANGLES, 29*29*2*3, GL_UNSIGNED_SHORT, NULL));
            //VBO::unbind(GL_ELEMENT_ARRAY_BUFFER);
            VAO::unbind();
            Program::noProgram();
        });

        /* Loop until the user closes the window */
        window.mainLoop();
    }

        glfwTerminate();

        return 0;
    }
