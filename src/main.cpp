#include <iostream>
#include <fstream>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <CImg.h>

#include "gl_core_3_3.h"
#include "window.h"
#include "program.h"
#include "vbo.h"
#include "vao.h"
#include "scenegraph.h"
#include "camera.h"

#include "debug.h"

// Initialize GLEW and GLFW
void init_libs(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Error : cannot initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback([] (int error, const char* description) { std::cerr << description << std::endl; });
}

// Build the shader program used in the project
Program buildShaderProgram() {
    std::ifstream fvs("shaders/per_fragment.vs");
    std::ifstream ffs("shaders/per_fragment.fs");
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
                                .with_uniform("m_proj", ProgramBuilder::mat4)
                                .with_uniform("m_world", ProgramBuilder::mat4)
                                .with_uniform("m_camera", ProgramBuilder::mat4)
                                .with_uniform("m_normalTransform", ProgramBuilder::mat3)
                                .with_uniform("v_lightPosition", ProgramBuilder::vec3)
                                .link();
    if(!p) {
        std::cerr << "Program link error : " << std::endl << p.info_log() << std::endl;
        exit(EXIT_FAILURE);
    }
    return p;
}

// Build the sphere mesh used for planets
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
        float phi = -M_PI/2 + (i+1)*dPhi;
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

// Compute the indices to draw the spheres in indexed mode
VBO build_sphere_indices() {
    const int nMeridians = 30;
    const int nParallels = 30;
    std::vector<GLshort> vec;
    
    for(int i = 0 ; i != (nParallels-1) ; ++i) {
        for(int j = 0 ; j != (nMeridians-1) ; ++j) {
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

void bind_camera_keys(Camera const& cam) {
}

void bind_input_callbacks(Engine::Window& window, Camera& cam) {
    window.registerKeyCallback(GLFW_KEY_ESCAPE, [&window] () { window.close(); });
    window.registerKeyCallback(' ', [&cam] () { cam.translate(Camera::Up, 1);  std::cout << "Going up!" << std::endl; });
    window.registerKeyCallback(GLFW_KEY_LEFT_CONTROL, [&cam] () { cam.translate(Camera::Down, 1);  std::cout << "Going down!" << std::endl; });
    window.registerKeyCallback('W', [&cam] () { cam.translate(Camera::Front, 1);  std::cout << "Going forward!" << std::endl; });
    window.registerKeyCallback('A', [&cam] () { cam.translate(Camera::Left, 1);  std::cout << "Going left!" << std::endl; });
    window.registerKeyCallback('S', [&cam] () { cam.translate(Camera::Back, 1);  std::cout << "Going back!" << std::endl; });
    window.registerKeyCallback('D', [&cam] () { cam.translate(Camera::Right, 1);  std::cout << "Going right!" << std::endl; });

    window.registerMouseCallback([&cam] (double x, double y) {
            static double prev_x = 0, prev_y = 0;
            double xoffset = x - prev_x, yoffset = y - prev_y;
            prev_x = x; prev_y = y;
            std::cout << "Mouse movement (" << xoffset << ", " << yoffset << ")" << std::endl; 
            cam.rotate(Camera::X, yoffset);
            cam.rotate(Camera::Y, xoffset);
        });
}

int main(int argc, char** argv) {
    init_libs(argc, argv);
    std::chrono::high_resolution_clock clock;

    std:: cout << "Loading textures (1/2)..." << std::endl;
    cimg_library::CImg<float> img("assets/earth_heightmap.jpg");
    std:: cout << "Loading textures (2/2)..." << std::endl;
    cimg_library::CImg<float> img2("assets/earth.png");
    std:: cout << "Done!" << std::endl;

    {
        // First, create the window
        Engine::WindowBuilder wb;
        Engine::Window window = wb.size(1280, 720)
                          .title("Projetlololol")
                          .build();
        if (!window) {
            std::cerr << "Error : cannot create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        std::cout << window.context_info() << std::endl;
        window.showCursor(false);
        
        // Then, the shader program
        glm::mat4 projMatrix = glm::perspective<float>(glm::radians(45.f), 1280.f/720.f, 0.1, 100),
                  worldMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5));
        glm::vec3 lightPosition(0, 5, 5);
        Program p = buildShaderProgram();
        p.use();
        Uniform<glm::mat4>* u1 = dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_proj"));
        std::cout << u1 << std::endl;
        u1->set(projMatrix);
        dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_world"))->set(worldMatrix);
        dynamic_cast<Uniform<glm::mat3>*>(p.getUniform("m_normalTransform"))->set(glm::transpose(glm::inverse(glm::mat3(worldMatrix))));
        dynamic_cast<Uniform<glm::vec3>*>(p.getUniform("v_lightPosition"))->set(lightPosition);
        window.setResizeCallback([&] (int w, int h) {
            GLV(glViewport(0, 0, w, h));
            projMatrix = glm::perspective<float>(45, (float)(w)/(float)(h), 0.1, 100);
            p.use();
            dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_proj"))->set(projMatrix);
        });
        // Create the sphere object
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

        // Fill the scene
        Camera camera;
        SceneGraph scene;
        IndexedObject* refSphere = new IndexedObject(worldMatrix, p, sphereIndices, sphereVao, 29*29*2*3);
        IndexedObject* leftSphere = new IndexedObject(glm::translate(glm::mat4(1.f), glm::vec3(-1, 0, 0)), p, sphereIndices, sphereVao, 29*29*2*3);
        refSphere->addChild(leftSphere);
        scene.addChild(refSphere);

        // Some more GL related stuff
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);
        glClearDepth(1.0f);

        // Install input callbacks
        bind_input_callbacks(window, camera);

        auto start = clock.now();
        // Finally, the render function
        window.setRenderCallback([&] () {
            glm::mat4 cameraMatrix = camera.mat();
            dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_camera"))->set(cameraMatrix);
            GLV(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            float t = std::chrono::duration<float>(clock.now() - start).count();
            glm::mat4 m(glm::translate(glm::rotate(glm::mat4(1), t, glm::vec3(0, 1, 0)), glm::vec3(-1, 0, 0)));
            leftSphere->set_transform(m);
            scene.render(); });

        window.mainLoop();
    }

        glfwTerminate();

        return 0;
    }
