#include "troll_engine.h"
#include "window.h"
#include "program.h"
#include "scenegraph.h"
#include "image.h"
#include "utility.h"
#include "ubo.h"
#include "gl_traits.h"

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
using namespace gl;
using namespace std;

struct TextOptions {
    bool softenEdges;
    bool outline;
    bool glow;
    float softenRange;
};

namespace Engine {
namespace traits {
    template <>
    struct uniform_block_types<TextOptions> {
        typedef type_list<bool, bool, bool, float> type;
    };
} // namespace traits
} // namespace Engine

glm::vec2 charTexCoords(char c) {
    if(c < 32 || c >= 127)
        throw std::runtime_error("Invalid character");
    int i = c - 32,
        x = i % 10,
        y = i / 10;
    float xf = x * (400.f / 4096.f), yf = y * (400.f / 4096.f);
    return glm::vec2(xf, yf);
}

vector<glm::vec2> stringTexCoords(std::string const& str) {
    vector<glm::vec2> uvs;
    uvs.reserve(str.size() * 4);
    float df = 400.f / 4096.f;
    for(auto it = str.begin() ; it != str.end() ; ++it) {
        glm::vec2 botLeft = charTexCoords(*it);
        cout << "Char " << *it << " texCoords: " << botLeft << endl;
        uvs.push_back(glm::vec2(botLeft.x, botLeft.y + df));
        uvs.push_back(glm::vec2(botLeft.x + df, botLeft.y + df));
        uvs.push_back(glm::vec2(botLeft.x + df, botLeft.y));
        uvs.push_back(botLeft);
    }
    return uvs;
}

int main(int, char**) {
    TrollEngine engine;
    GLFWWindow win(1280, 720, "TrollEngine GLFW Window", true, true);
    win.setResizeCallback([] (int w, int h) { glViewport(0, 0, w, h); });
    win.showCursor(false);

    std::cout << win.context_info() << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);

    ProgramBuilder pb;
    pb.vertexShader("vs.glsl")
      .fragmentShader("fs.glsl")
      .uniform("tex", ProgramBuilder::UniformType::Int)
      .uniform("modelView", ProgramBuilder::UniformType::Mat4)
      .uniform("camera", ProgramBuilder::UniformType::Mat4)
      .uniform("projection", ProgramBuilder::UniformType::Mat4);
    auto program = std::move(pb.build());

    UBO<TextOptions> opts;
    opts.data = {true, false, false, 0.1f};
    opts.upload_std140();
    auto optsIndex = program.getUniformBlockIndex("Options");
    program.uniformBlockBinding(optsIndex, 0);
    opts.bindBase(GL_UNIFORM_BUFFER, 0);

    auto texImg = GreyscaleImage::load("distanceField.png");
    auto tex = Texture::fromImage(texImg);

    glm::mat4 p = glm::perspective(glm::radians(55.f), 16.f / 9.f, 0.1f, 100.f),
              t = glm::translate(glm::mat4(1.f), glm::vec3(-6.f, 0.f, -7.f)),
              r = glm::rotate(glm::mat4(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f)),
              m = t * r;

    SceneGraph scene;
    vector<glm::vec2> uvs = stringTexCoords("Hello world!");
    auto quadStrip = Mesh::quadStrip(uvs);
    auto quadStripObj = quadStrip->instantiate(m, &program, &tex);
    scene.addChild(quadStripObj);

    program.use();
    auto ubTexture = program.getUniform("tex"),
         ubModelView = program.getUniform("modelView"),
         ubProjection = program.getUniform("projection");
    auto uTexture = dynamic_cast<Uniform<int>*>(ubTexture);
    auto uModelView = dynamic_cast<Uniform<glm::mat4>*>(ubModelView);
    auto uProjection = dynamic_cast<Uniform<glm::mat4>*>(ubProjection);
    uTexture->set(0);
    uModelView->set(m);
    uProjection->set(p);

    win.registerKeyCallback('A', [&] () {
            opts.data.softenRange = min(opts.data.softenRange + 0.1f, 0.95f);
            opts.upload_std140();
        });
    win.registerKeyCallback('E', [&] () {
            opts.data.softenRange = max(opts.data.softenRange - 0.1f, 0.05f);
            opts.upload_std140();
        });

    while(!win.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();
        win.swapBuffers();
        win.pollEvents();
    }
}
