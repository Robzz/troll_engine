#include "glwidget.h"

using namespace Engine;

GLWidget::GLWidget(QWidget* parent) :
    Qt5Surface(parent, 640, 480, true, true),
    m_engine(),
    m_program(),
    m_triangle(),
    m_scene()
{ }

void GLWidget::initializeGL() {
    makeCurrent();
    glClearColor(0.f, 0.f, 0.f, 1.f);

    ProgramBuilder pb;
    pb.vertexShader("vs.glsl")
      .fragmentShader("fs.glsl")
      .uniform("triangleColor", ProgramBuilder::UniformType::Vec3);
    m_program = std::move(pb.build());

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
    m_triangle = mb.build_mesh();

    m_scene.addChild(m_triangle->instantiate(glm::mat4(1.f), &m_program));

    m_program.use();
    auto ub = m_program.getUniform("triangleColor");
    auto u = dynamic_cast<Uniform<glm::vec3>*>(ub);
    u->set(glm::vec3(1.f, 0.f, 0.f));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene.render();
}

void GLWidget::setColor(float r, float g, float b) {
    makeCurrent();
    m_program.use();
    auto ub = m_program.getUniform("triangleColor");
    auto u = dynamic_cast<Uniform<glm::vec3>*>(ub);
    u->set(glm::vec3(r, g, b));
    requestUpdate();
}
