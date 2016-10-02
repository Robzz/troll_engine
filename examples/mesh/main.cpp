#include "troll_engine.h"
#include "window.h"
#include "program.h"
#include "sceneimporter.h"
#include "scenegraph.h"
#include "image.h"
#include "camera.h"
#include "utility.h"
#include "ubo.h"
#include "gl_traits.h"

#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;

struct Material {
    glm::vec3 Ks;
    float alpha;
    glm::vec3 Ka;
};

struct Light {
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 position;
};

namespace Engine {
namespace traits {
    template <>
    struct uniform_block_types<Material> {
        typedef type_list<glm::vec3, float, glm::vec3> type;
    };

    template <>
    struct uniform_block_types<Light> {
        typedef type_list<glm::vec3, glm::vec3, glm::vec3> type;
    };
} // namespace traits
} // namespace Engine

int main(int, char**) {
    TrollEngine engine;
    GLFWWindow win(1280, 720, "TrollEngine GLFW Window", true, true);
    win.setResizeCallback([] (int w, int h) { glViewport(0, 0, w, h); });
    win.showCursor(false);

    std::cout << win.context_info() << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);

    UBO<Material> uboMat;
    uboMat.data.Ka = glm::vec3(0.1f, 0.1f, 0.1f);
    uboMat.data.Ks = glm::vec3(0.4f, 0.4f, 0.4f);
    uboMat.data.alpha = 2.2f;
    uboMat.upload_std140();

    UBO<Light> uboLight;
    uboLight.data.diffuseColor = glm::vec3(0.9f, 0.9f, 0.9f);
    uboLight.data.specularColor = glm::vec3(0.9f, 0.9f, 0.7f);
    uboLight.data.position = glm::vec3(5.f, 5.f, -5.f);
    uboLight.upload_std140();

    Camera<TransformMat> camera;

    ProgramBuilder pb;
    pb.vertexShader("vs.glsl")
      .fragmentShader("fs.glsl")
      .uniform("tex", ProgramBuilder::UniformType::Int)
      .uniform("modelView", ProgramBuilder::UniformType::Mat4)
      .uniform("camera", ProgramBuilder::UniformType::Mat4)
      .uniform("cameraWorldPos", ProgramBuilder::UniformType::Vec3)
      .uniform("projection", ProgramBuilder::UniformType::Mat4)
      .uniform("normalTransform", ProgramBuilder::UniformType::Mat3);
    auto program = std::move(pb.build());

    auto matIndex = program.getUniformBlockIndex("Material"),
         lightIndex = program.getUniformBlockIndex("Light");
    program.uniformBlockBinding(matIndex, 0);
    program.uniformBlockBinding(lightIndex, 1);

    uboMat.bindBase(GL_UNIFORM_BUFFER, 0);
    uboLight.bindBase(GL_UNIFORM_BUFFER, 1);

    auto tex = Image("metal.jpg").to_texture();

    SceneImporter imp;
    imp.readFile("teapot.obj", SceneImporter::PostProcess::JoinVertices | SceneImporter::PostProcess::GenerateNormals | SceneImporter::PostProcess::FlipWindingOrder);
    std::unique_ptr<Mesh> teapot = imp.instantiateMesh(*imp.meshes()[0]);
    std::unique_ptr<Mesh> top = imp.instantiateMesh(*imp.meshes()[1]);

    SceneGraph scene;
    auto teapotNode = teapot->instantiate(glm::mat4(1.f), &program);
    auto topNode = top->instantiate(glm::mat4(1.f), &program);
    teapotNode->set_texture(tex);
    topNode->set_texture(tex);
    scene.addChild(teapotNode);
    scene.addChild(topNode);

    float theta = 0.f, dt = 0.1f;
    glm::mat4 p = glm::perspective(55.f, 16.f / 9.f, 0.1f, 100.f),
              t = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.25f, -1.f)),
              s = glm::scale(glm::mat4(1.f), glm::vec3(1.f/100, -1.f/100, 1.f/100)),
              r = glm::rotate(glm::mat4(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f)),
              m = t * r * s;
    glm::mat3 nt = glm::inverseTranspose(glm::mat3(m));
    glm::vec3 cameraWorldPos = glm::vec3(camera.transform().matrix()[3]);

    program.use();
    auto ubTexture = program.getUniform("tex"),
         ubModelView = program.getUniform("modelView"),
         ubCamera = program.getUniform("camera"),
         ubCameraWorldPos = program.getUniform("cameraWorldPos"),
         ubProjection = program.getUniform("projection"),
         ubNormalTransform = program.getUniform("normalTransform");
    auto uTexture = dynamic_cast<Uniform<int>*>(ubTexture);
    auto uModelView = dynamic_cast<Uniform<glm::mat4>*>(ubModelView);
    auto uCamera = dynamic_cast<Uniform<glm::mat4>*>(ubCamera);
    auto uCameraWorldPos = dynamic_cast<Uniform<glm::vec3>*>(ubCameraWorldPos);
    auto uProjection = dynamic_cast<Uniform<glm::mat4>*>(ubProjection);
    auto uNormalTransform = dynamic_cast<Uniform<glm::mat3>*>(ubNormalTransform);
    uTexture->set(0);
    uModelView->set(m);
    uCamera->set(camera.transform().matrix());
    uCameraWorldPos->set(cameraWorldPos);
    uProjection->set(p);
    uNormalTransform->set(nt);

    win.registerKeyCallback('E', [&] () {
            theta += dt;
            r = glm::rotate(glm::mat4(1.f), theta, glm::vec3(0.f, 1.f, 0.f)),
            m = t * r * s;
            uModelView->set(m);
        });
    win.registerKeyCallback('A', [&] () {
            theta -= dt;
            r = glm::rotate(glm::mat4(1.f), theta, glm::vec3(0.f, 1.f, 0.f)),
            m = t * r * s;
            uModelView->set(m);
        });

    while(!win.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();
        win.swapBuffers();
        win.pollEvents();
    }
}
