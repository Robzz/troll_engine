#include "scenegraph.h"
#include "vao.h"
#include "vbo.h"
#include "program.h"
#include "glm/gtc/matrix_inverse.hpp"

Node::Node(glm::mat4 const& position, std::string const& name, bool enabled) :
    m_enabled(enabled),
    m_position(position),
    m_children(),
    m_id(0),
    m_name(name)
{ }

Node::~Node() {
    for(auto it = m_children.begin() ; it != m_children.end() ; ++it) {
        delete it->second;        
    }
}

int Node::addChild(Node* n) {
    int i = nextId();
    m_children.insert(std::pair<int, Node*>(i, n));
    return i;
}

void Node::removeChildById(int id) {
    try {
        delete m_children.at(id);
        m_children.erase(id);
    }
    catch(std::out_of_range) { }
}

void Node::removeChildsByName(std::string const& name) {
    for(auto it = m_children.begin() ; it != m_children.end() ; ++it) {
        if(it->second->m_name == name) {
            removeChildById(it->first);
        }
    }
}

glm::mat4 Node::transform() const {
    return m_position;
}

void Node::set_transform(glm::mat4 const& m) {
    m_position = m;
}

int Node::nextId() {
    return m_id++;
}

SceneGraph::SceneGraph() :
    Node(),
    m_matrixStack()
{ }

SceneGraph::~SceneGraph() { }

void SceneGraph::render() {
    for(auto it = m_children.begin() ; it != m_children.end() ; ++it) {
        Node* n = it->second;
        render(n);
    }
}

void SceneGraph::render(int id) {
    try {
        Node* n = m_children.at(id);
        render(n);
    }
    catch(std::out_of_range) { }
}

void SceneGraph::render(Node* n) {
    m_matrixStack.push(n->m_position);
    DrawableNode* dn = dynamic_cast<DrawableNode*>(n);
    /* If the node inherits from DrawableNode, draw it.
     * Otherwise, just draw it's children. */
    if(dn) {
        dn->draw(m_matrixStack.top());
    }
    for(auto it = n->m_children.begin() ; it != n->m_children.end() ; ++it) {
        render(it->second);
    }
    m_matrixStack.pop();
}

DrawableNode::DrawableNode(glm::mat4 const& position, Texture const& tex) :
    Node(position),
    m_tex(tex)
{ }

Object::Object(glm::mat4 const& position, Program& p, VAO& vao, int n_primitives,
               Texture const& tex, GLenum primitiveMode) :
    DrawableNode(position, tex),
    m_n_primitives(n_primitives),
    m_primitiveMode(primitiveMode),
    m_program(p),
    m_vao(vao)
{ }

Object::~Object() { }

void Object::draw(glm::mat4 const& m) {
    m_program.use();
    auto u = dynamic_cast<Uniform<glm::mat4>*>(m_program.getUniform("m_world"));
    if(u)
        u->set(m);
    m_program.uploadUniforms();
    m_vao.bind();
    m_tex.bind();
    GLV(glDrawArrays(m_primitiveMode, 0, m_n_primitives));
    Texture::unbind();
    VAO::unbind();
    Program::noProgram();
}

IndexedObject::IndexedObject(glm::mat4 const& position, Program& p, VBO& ebo, VAO& vao, int nVertices,
                             Texture const& tex, GLenum indexType, GLenum primitiveMode) :
    DrawableNode(position, tex),
    m_program(p),
    m_ebo(ebo),
    m_vao(vao),
    m_nVertices(nVertices),
    m_indexType(indexType),
    m_primitiveMode(primitiveMode)
{ }

IndexedObject::~IndexedObject() {

}

void IndexedObject::draw(glm::mat4 const& m) {
    m_program.use();
    // Remove scaling from m or it will apply to children too
    auto u = dynamic_cast<Uniform<glm::mat4>*>(m_program.getUniform("m_world"));
    if(u)
        u->set(m);
    m_program.uploadUniforms();
    m_vao.bind();
    m_ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    m_tex.bind();
    GLV(glDrawElements(m_primitiveMode, m_nVertices, m_indexType, NULL);)
    Texture::unbind();
    VBO::unbind(GL_ELEMENT_ARRAY_BUFFER);
    VAO::unbind();
    Program::noProgram();
}
