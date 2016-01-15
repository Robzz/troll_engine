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

VBO build_sphere_texcoords() {
    const int nMeridians = 30;
    const int nParallels = 30;
    std::vector<glm::vec2> vec;
    for(int i = 0 ; i != nParallels ; ++i)
    for(int j = 0 ; j != nMeridians ; ++j) {
        vec.push_back(glm::vec2(static_cast<float>(j) / nMeridians,
                                -static_cast<float>(i) / nParallels));
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

VBO build_plane_mesh() {
    std::vector<glm::vec3> vec;
    vec.push_back(glm::vec3(-1,  1, -1));
    vec.push_back(glm::vec3(-1, -1, -1));
    vec.push_back(glm::vec3( 1, -1, -1));
    vec.push_back(glm::vec3(-1,  1, -1));
    vec.push_back(glm::vec3( 1, -1, -1));
    vec.push_back(glm::vec3( 1,  1, -1));

    VBO v;
    v.upload_data(vec);
    return v;
}

VBO build_plane_texCoords() {
    std::vector<glm::vec2> vec;
    vec.push_back(glm::vec2(0,  1));
    vec.push_back(glm::vec2(0,  0));
    vec.push_back(glm::vec2(1,  0));
    vec.push_back(glm::vec2(0,  1));
    vec.push_back(glm::vec2(1,  0));
    vec.push_back(glm::vec2(1,  1));

    VBO v;
    v.upload_data(vec);
    return v;
}

void bind_input_callbacks(Engine::Window& window, Camera& cam) {
    window.registerKeyCallback(GLFW_KEY_ESCAPE, [&window] () { window.close(); });
    window.registerKeyCallback(GLFW_KEY_LEFT_CONTROL, [&cam] () { cam.translate(Camera::Down, 1); });
    window.registerKeyCallback(' ', [&cam] () { cam.translate(Camera::Up, 1); });
    window.registerKeyCallback('W', [&cam] () { cam.translate(Camera::Front, 1); });
    window.registerKeyCallback('A', [&cam] () { cam.translate(Camera::Left, 1); });
    window.registerKeyCallback('S', [&cam] () { cam.translate(Camera::Back, 1); });
    window.registerKeyCallback('D', [&cam] () { cam.translate(Camera::Right, 1); });

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

    {
        // First, create the window
        Engine::WindowBuilder wb;
        Engine::Window window = wb.size(1280, 720)
                                .title("Projetlololol")
                                .vsync(false)
                                .build();
        if (!window) {
            std::cerr << "Error : cannot create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        std::cout << window.context_info() << std::endl;
        window.showCursor(false);
        
        // Then, the shader program
        glm::mat4 projMatrix = glm::perspective<float>(glm::radians(45.f), 1280.f/720.f, 0.1, 1000),
                  worldMatrix = glm::scale(glm::mat4(1.f), glm::vec3(30, 30, 30));
        glm::vec3 lightPosition(0, 0, 0);
        std::vector<std::pair<std::string, ProgramBuilder::UniformType>> uniforms_p1, uniforms_p2, uniforms_p3;

        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_proj", ProgramBuilder::mat4));
        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_world", ProgramBuilder::mat4));
        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_camera", ProgramBuilder::mat4));
        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_normalTransform", ProgramBuilder::mat3));
        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("ambient_intensity", ProgramBuilder::float_));
        uniforms_p1.push_back(std::pair<std::string, ProgramBuilder::UniformType>("u_tex", ProgramBuilder::int_));
        uniforms_p2.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_proj", ProgramBuilder::mat4));
        uniforms_p2.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_world", ProgramBuilder::mat4));
        uniforms_p2.push_back(std::pair<std::string, ProgramBuilder::UniformType>("m_camera", ProgramBuilder::mat4));
        uniforms_p2.push_back(std::pair<std::string, ProgramBuilder::UniformType>("u_tex", ProgramBuilder::int_));
        Program planetProgram = buildShaderProgram("shaders/tex_lighting.vs", "shaders/tex_lighting.fs", uniforms_p1),
                sunProgram = buildShaderProgram("shaders/texture.vs", "shaders/texture.fs", uniforms_p2);

        dynamic_cast<Uniform<glm::mat4>*>(planetProgram.getUniform("m_proj"))->set(projMatrix);;
        dynamic_cast<Uniform<float>*>(planetProgram.getUniform("ambient_intensity"))->set(0.02);

        dynamic_cast<Uniform<glm::mat4>*>(sunProgram.getUniform("m_proj"))->set(projMatrix);;
        dynamic_cast<Uniform<glm::mat4>*>(sunProgram.getUniform("m_world"))->set(worldMatrix);

        window.setResizeCallback([&] (int w, int h) {
            GLV(glViewport(0, 0, w, h));
            projMatrix = glm::perspective<float>(45, (float)(w)/(float)(h), 0.1, 1000);
            planetProgram.use();
            dynamic_cast<Uniform<glm::mat4>*>(planetProgram.getUniform("m_proj"))->set(projMatrix);
            sunProgram.use();
            dynamic_cast<Uniform<glm::mat4>*>(sunProgram.getUniform("m_proj"))->set(projMatrix);
        });

        VBO sphereVbo = build_sphere_mesh(),
            sphereTexCoords = build_sphere_texcoords(),
            sphereIndices = build_sphere_indices(),
            planeVbo = build_plane_mesh(),
            planeTexCoords = build_plane_texCoords();
        VAO planetVao, sunVao, planeVao;

        // Setup vertex attributes
        GLint posIndex       = planetProgram.getAttributeLocation("v_position");
        GLint normalIndex    = planetProgram.getAttributeLocation("v_normal");
        GLint texCoordsIndex = planetProgram.getAttributeLocation("v_texCoord");
        planetVao.enableVertexAttribArray(posIndex);
        planetVao.enableVertexAttribArray(normalIndex);
        planetVao.enableVertexAttribArray(texCoordsIndex);
        planetVao.vertexAttribPointer(sphereVbo, posIndex, 3);
        planetVao.vertexAttribPointer(sphereVbo, normalIndex, 3, 0 , NULL, GL_FLOAT, GL_TRUE);
        planetVao.vertexAttribPointer(sphereTexCoords, texCoordsIndex, 2);

        posIndex       = sunProgram.getAttributeLocation("v_position");
        texCoordsIndex = sunProgram.getAttributeLocation("v_texCoord");
        sunVao.enableVertexAttribArray(posIndex);
        sunVao.enableVertexAttribArray(texCoordsIndex);
        sunVao.vertexAttribPointer(sphereVbo, posIndex, 3);
        sunVao.vertexAttribPointer(sphereTexCoords, texCoordsIndex, 2);

        planeVao.enableVertexAttribArray(posIndex);
        planeVao.enableVertexAttribArray(texCoordsIndex);
        planeVao.vertexAttribPointer(planeVbo, posIndex, 3);
        planeVao.vertexAttribPointer(planeTexCoords, texCoordsIndex, 2);

        // Fill the scene
        Camera camera;
        camera.translate(Camera::Back, 100);
        SceneGraph scene;
        glActiveTexture(GL_TEXTURE0);
        Texture earthTex(   Texture::from_image("assets/texture_earth.bmp")),
                mercuryTex( Texture::from_image("assets/texture_mercury.bmp")),
                venusTex(   Texture::from_image("assets/texture_venus_surface.bmp")),
                marsTex(    Texture::from_image("assets/texture_mars.bmp")),
                jupiterTex( Texture::from_image("assets/texture_jupiter.bmp")),
                saturnTex(  Texture::from_image("assets/texture_saturn.bmp")),
                uranusTex(  Texture::from_image("assets/texture_uranus.bmp")),
                neptuneTex( Texture::from_image("assets/texture_neptune.bmp")),
                sunTex(     Texture::from_image("assets/texture_sun.bmp")),
                skyboxFront(Texture::from_image("assets/stars_fr.bmp")),
                skyboxBack( Texture::from_image("assets/stars_bk.bmp")),
                skyboxLeft( Texture::from_image("assets/stars_lf.bmp")),
                skyboxRight(Texture::from_image("assets/stars_rt.bmp")),
                skyboxUp(   Texture::from_image("assets/stars_up.bmp")),
                skyboxDown( Texture::from_image("assets/stars_dn.bmp"));
        
        Object *skyUp    = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxUp),
               *skyDown  = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxDown),
               *skyLeft  = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxLeft),
               *skyRight = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxRight),
               *skyFront = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxFront),
               *skyBack  = new Object(glm::mat4(1), sunProgram, planeVao, 6, skyboxBack);
        IndexedObject* sun = new IndexedObject(worldMatrix, sunProgram, sphereIndices, planetVao, 29*29*2*3, sunTex);
        Planet* earth = new Planet(1, 50, 1, 1, planetProgram, sphereIndices, planetVao, 29*29*2*3, earthTex);
        Planet* mercury = new Planet(0.3829, 30, 0.2408, 58.64, planetProgram, sphereIndices, planetVao, 29*29*2*3, mercuryTex);
        Planet* venus = new Planet(0.9499, 40, 0.6152, -243, planetProgram, sphereIndices, planetVao, 29*29*2*3, venusTex);
        Planet* mars = new Planet(0.53, 60, 1.881, 1.026, planetProgram, sphereIndices, planetVao, 29*29*2*3, marsTex);
        Planet* jupiter = new Planet(11.21, 70, 11.86, 0.4135, planetProgram, sphereIndices, planetVao, 29*29*2*3, jupiterTex);
        Planet* saturn = new Planet(9.449, 80, 29.46, 0.4396, planetProgram, sphereIndices, planetVao, 29*29*2*3, saturnTex);
        Planet* uranus = new Planet(4.007, 90, 84.02, 0.7183, planetProgram, sphereIndices, planetVao, 29*29*2*3, uranusTex);
        Planet* neptune = new Planet(3.883, 100, 164.8, 0.6713, planetProgram, sphereIndices, planetVao, 29*29*2*3, neptuneTex);
        sun->addChild(earth);
        sun->addChild(mercury);
        sun->addChild(venus);
        sun->addChild(mars);
        sun->addChild(jupiter);
        sun->addChild(saturn);
        sun->addChild(uranus);
        sun->addChild(neptune);
        scene.addChild(sun);

        // Some more GL related stuff
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);
        glClearDepth(1.0f);

        // Install input callbacks
        bind_input_callbacks(window, camera);

        // Finally, the render function
        window.setRenderCallback([&] () {
            glm::mat4 cameraMatrix = camera.mat();
            planetProgram.use();
            dynamic_cast<Uniform<glm::mat4>*>(planetProgram.getUniform("m_camera"))->set(cameraMatrix);
            dynamic_cast<Uniform<glm::mat3>*>(planetProgram.getUniform("m_normalTransform"))->set(glm::inverseTranspose(glm::mat3(worldMatrix)));
            sunProgram.use();

            GLV(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // First draw the skybox with depth test off
            glDisable(GL_DEPTH_TEST);
            sunProgram.use();
            glm::mat4 m(cameraMatrix), m2,
                      rz(glm::rotate(glm::mat4(1), 3.141592f / 2, glm::vec3(1, 0, 0))),
                      ry(glm::rotate(glm::mat4(1), 3.141592f / 2, glm::vec3(0, 1, 0)));
            m[3] = glm::vec4(0, 0, 0, 1);
            dynamic_cast<Uniform<glm::mat4>*>(sunProgram.getUniform("m_camera"))->set(glm::mat4(1));
            skyFront->draw(m);
            m2 = m * rz;
            skyUp->draw(glm::mat4(m2));
            m2 = m2 * rz;
            skyBack->draw(glm::mat4(m2));
            m2 = m2 * rz;
            skyDown->draw(glm::mat4(m2));
            skyLeft->draw(m * ry);
            skyRight->draw(m * glm::transpose(ry));
            glEnable(GL_DEPTH_TEST);

            dynamic_cast<Uniform<glm::mat4>*>(sunProgram.getUniform("m_camera"))->set(cameraMatrix);
            scene.render(); });

        window.mainLoop();
    }

        glfwTerminate();

        return 0;
    }
