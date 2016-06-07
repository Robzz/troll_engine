#include "troll_engine.h"
#include "window.h"
#include "program.h"
#include "sceneimporter.h"
#include "scenegraph.h"

#include <iostream>

using namespace Engine;

int main(int argc, char** argv) {
    TrollEngine engine;
    GLFWWindow win(1280, 720, "TrollEngine GLFW Window", true, true);
    win.setResizeCallback([] (int w, int h) { glViewport(0, 0, w, h); });
    win.showCursor(false);

    std::cout << win.context_info() << std::endl;

    glClearColor(0.f, 0.f, 0.f, 1.f);

    ProgramBuilder pb;
    pb.vertexShader("vs.glsl")
      .fragmentShader("fs.glsl")
      .uniform("triangleColor", ProgramBuilder::UniformType::Vec3);
    auto program = std::move(pb.build());

    MeshBuilder mb;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned char> indices;
    vertices.push_back(glm::vec3(-0.5, -0.5, 0));
    vertices.push_back(glm::vec3(-0.5,  0.5, 0));
    vertices.push_back(glm::vec3( 0.5,  0.5, 0));
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    mb.vertices(vertices)
      .faces(std::move(indices));
    auto triangle = mb.build_mesh();

    SceneGraph scene;
    scene.addChild(triangle->instantiate(glm::mat4(1.f), &program));

    program.use();
    auto ub = program.getUniform("triangleColor");
    auto u = dynamic_cast<Uniform<glm::vec3>*>(ub);
    u->set(glm::vec3(1.f, 0.f, 0.f));

    while(!win.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();
        win.swapBuffers();
        win.pollEvents();
    }
}
