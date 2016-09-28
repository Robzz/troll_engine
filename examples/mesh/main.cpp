#include "troll_engine.h"
#include "window.h"
#include "program.h"
#include "sceneimporter.h"
#include "scenegraph.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

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
      .uniform("teapotColor", ProgramBuilder::UniformType::Vec3)
      .uniform("modelView", ProgramBuilder::UniformType::Mat4);
    auto program = std::move(pb.build());

    SceneImporter imp;
    imp.readFile("teapot.obj", SceneImporter::PostProcess::JoinVertices);
    std::unique_ptr<Mesh> teapot = imp.instantiateMesh(*imp.meshes()[0]);
    std::cout << *teapot << std::endl;

    SceneGraph scene;
    scene.addChild(teapot->instantiate(glm::mat4(1.f), &program));

    float theta = 0.f, dt = 0.1f;
    glm::mat4 p = glm::perspective(55.f, 16.f / 9.f, 0.1f, 100.f),
              t = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.25f, -1.f)),
              s = glm::scale(glm::mat4(1.f), glm::vec3(1.f/100, -1.f/100, 1.f/100)),
              r = glm::rotate(glm::mat4(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f)),
              m = p * t * r * s;

    program.use();
    auto ubColor = program.getUniform("teapotColor"),
         ubModelView = program.getUniform("modelView");
    auto uColor = dynamic_cast<Uniform<glm::vec3>*>(ubColor);
    auto uModelView = dynamic_cast<Uniform<glm::mat4>*>(ubModelView);
    uColor->set(glm::vec3(0.6f, 0.3f, 0.2f));
    uModelView->set(m);

    win.registerKeyCallback('E', [&] () {
            theta += dt;
            r = glm::rotate(glm::mat4(1.f), theta, glm::vec3(0.f, 1.f, 0.f)),
            m = p * t * r * s;
            uModelView->set(m);
        });
    win.registerKeyCallback('A', [&] () {
            theta -= dt;
            r = glm::rotate(glm::mat4(1.f), theta, glm::vec3(0.f, 1.f, 0.f)),
            m = p * t * r * s;
            uModelView->set(m);
        });

    while(!win.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();
        win.swapBuffers();
        win.pollEvents();
    }
}
