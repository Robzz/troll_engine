#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "gl_core_3_3.h"
#include "window.h"
#include "program.h"
#include "vbo.h"
#include "vao.h"
#include "scenegraph.h"
#include "camera.h"
#include "texture.h"
#include "planet.h"
#include "debug.h"
#include "obj.h"

// TODO : wrap this in the lib
// Initialize GLEW and GLFW
void init_libs(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Error : cannot initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback([] (int error, const char* description) { std::cerr << description << std::endl; });
}

// Build the shader program used in the project
Program buildShaderProgram(std::string const& vs_file, std::string const& fs_file, std::vector<std::pair<std::string, ProgramBuilder::UniformType>> const& uniforms) {
    std::ifstream fvs(vs_file);
    std::ifstream ffs(fs_file);
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
    ProgramBuilder pb = ProgramBuilder().attach_shader(vs)
                                        .attach_shader(fs);
    for(auto it = uniforms.begin() ; it != uniforms.end() ; ++it) {
        pb = pb.with_uniform((*it).first, (*it).second);
    }
    Program p = pb.link();

    if(!p) {
        std::cerr << "Program link error : " << std::endl << p.info_log() << std::endl;
        exit(EXIT_FAILURE);
    }
    return p;
}

void bind_input_callbacks(Engine::Window& window, Camera& cam) {
    window.registerKeyCallback(GLFW_KEY_ESCAPE, [&window] () { window.close(); });
    window.registerKeyCallback(GLFW_KEY_LEFT_CONTROL, [&cam] () { cam.translate(Camera::Down, 1); });
    window.registerKeyCallback(' ', [&cam] () { cam.translate(Camera::Up, 5); });
    window.registerKeyCallback('W', [&cam] () { cam.translate(Camera::Front, 5); });
    window.registerKeyCallback('A', [&cam] () { cam.translate(Camera::Left, 5); });
    window.registerKeyCallback('S', [&cam] () { cam.translate(Camera::Back, 5); });
    window.registerKeyCallback('D', [&cam] () { cam.translate(Camera::Right, 5); });
    window.registerKeyCallback('Q', [&cam] () { cam.rotate(Camera::Z, 15); });
    window.registerKeyCallback('E', [&cam] () { cam.rotate(Camera::Z, -15); });

    window.registerMouseCallback([&cam] (double x, double y) {
            static double prev_x = 0, prev_y = 0;
            double xoffset = x - prev_x, yoffset = y - prev_y;
            prev_x = x; prev_y = y;
            cam.rotate(Camera::X, yoffset);
            cam.rotate(Camera::Y, xoffset);
        });
}

int main(int argc, char** argv) {
    init_libs(argc, argv);
    Obj obj = ObjReader().file("assets/cube.obj").read();
    IndexedMesh<unsigned int>* mesh = obj.get_group("default");

    std::cout << "Got " << obj.groups().size() << std::endl;
    for(auto& p: obj.groups()) {
        std::cout << "Group " << p.first << " :" << std::endl
                  << p.second.n_vertices()  << " vertices"  << std::endl
                  << p.second.n_texCoords() << " texCoords" << std::endl
                  << p.second.n_normals()   << " normals"   << std::endl
                  << p.second.n_indices()   << " indices"   << std::endl;
        std::cout << "Vertices : " << std::endl << p.second.vertices();
        std::cout << "Normals : "  << std::endl << p.second.normals();
        std::cout << "Indices : "  << std::endl << p.second.indices();
    }

    {
        // First, create the window
        Engine::WindowBuilder wb;
        Engine::Window window = wb.size(1280, 720)
                                .title("Buddha")
                                .vsync(false)
                                .build();
        if (!window) {
            std::cerr << "Error : cannot create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        std::cout << window.context_info() << std::endl;
        window.showCursor(false);

        VBO coords, normals, indices;
        coords.upload_data(mesh->vertices());
        normals.upload_data(mesh->normals());
        indices.upload_data(mesh->indices());
        
        // Then, the shader programs
        glm::mat4 projMatrix = glm::perspective<float>(glm::radians(45.f), 1280.f/720.f, 0.1, 1000),
                  worldMatrix = glm::mat4(1.f);
        glm::vec3 lightPosition(5, 15, -15);
        std::vector<std::pair<std::string, ProgramBuilder::UniformType>> uniforms_p;

        // TODO : Heh. Maybe not *so* elegant after all?
        uniforms_p.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_proj", ProgramBuilder::mat4));
        uniforms_p.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_world", ProgramBuilder::mat4));
        uniforms_p.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_camera", ProgramBuilder::mat4));
        uniforms_p.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_normalTransform", ProgramBuilder::mat3));
        uniforms_p.push_back(std::pair<std::string, ProgramBuilder::UniformType>("ambient_intensity", ProgramBuilder::float_));
        Program p(buildShaderProgram("shaders/per_fragment.vs", "shaders/per_fragment.fs", uniforms_p));
        dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_proj"))->set(projMatrix);;
        dynamic_cast<Uniform<float>*>(p.getUniform("ambient_intensity"))->set(0.2);
        dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_world"))->set(worldMatrix);

        window.setResizeCallback([&] (int w, int h) {
            // TODO : wrap the GL call away
            GLV(glViewport(0, 0, w, h));
            projMatrix = glm::perspective<float>(45, (float)(w)/(float)(h), 0.1, 1000);
            p.use();
            dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_proj"))->set(projMatrix);
        });

        // Setup vertex attributes
        VAO vao;
        GLint posIndex       = p.getAttributeLocation("v_position");
        GLint normalIndex    = p.getAttributeLocation("v_normal");
        vao.enableVertexAttribArray(posIndex);
        vao.vertexAttribPointer(coords, posIndex, 4, 0, 0);
        vao.enableVertexAttribArray(normalIndex);
        vao.vertexAttribPointer(normals, normalIndex, 3, 0, 0);

        // Fill the scene
        Camera camera;
        camera.translate(Camera::Back, 10);
        SceneGraph scene;
        IndexedObject* buddha = new IndexedObject(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5)), p, indices, vao, mesh->indices().size(), Texture::noTexture(), GL_UNSIGNED_INT);
        scene.addChild(buddha);
        
        // TODO : this must go
        // Some more GL related stuff
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);
        glClearColor(0.4f, 0.4f, 0.4f, 1.f);
        glClearDepth(1.0f);

        // Install input callbacks
        bind_input_callbacks(window, camera);

        // Finally, the render function
        window.setRenderCallback([&] () {
            glm::mat4 cameraMatrix = camera.mat();
            p.use();
            dynamic_cast<Uniform<glm::mat4>*>(p.getUniform("m_camera"))->set(cameraMatrix);
            dynamic_cast<Uniform<glm::mat3>*>(p.getUniform("m_normalTransform"))->set(glm::inverseTranspose(glm::mat3(worldMatrix)));

            // TODO : this too
            GLV(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            scene.render(); });

        window.mainLoop();
    }

        // TODO : yep, this too
        glfwTerminate();

        return 0;
    }
