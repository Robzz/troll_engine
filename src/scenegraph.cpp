#include "scenegraph.h"
#include "vao.h"
#include "vbo.h"
#include "program.h"

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

int Node::nextId() {
    return m_id++;
}

SceneGraph::SceneGraph() :
    Node()
{ }

SceneGraph::~SceneGraph() { }

void SceneGraph::render() {
    for(auto it = m_children.begin() ; it != m_children.end() ; ++it) {
        Node* n = it->second;
        m_matrixStack.push(n->m_position);
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
        dn->draw();
    }
    for(auto it = n->m_children.begin() ; it != n->m_children.end() ; ++it) {
        render(it->second);
    }
    m_matrixStack.pop();
}

DrawableNode::DrawableNode(glm::mat4 const& position) :
    Node(position)
{ }

IndexedObject::IndexedObject(glm::mat4 position, Program& p, VBO& ebo, VAO& vao, unsigned int nVertices,
                             GLenum indexType, GLenum primitiveMode) :
    DrawableNode(position),
    m_program(p),
    m_ebo(ebo),
    m_vao(vao),
    m_nVertices(nVertices),
    m_indexType(indexType),
    m_primitiveMode(primitiveMode)
{ }

IndexedObject::~IndexedObject() {

}

void IndexedObject::draw() {
    m_program.use();
    m_vao.bind();
    m_ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    GLV(glDrawElements(m_primitiveMode, m_nVertices, m_indexType, NULL);)
    VBO::unbind(GL_ELEMENT_ARRAY_BUFFER);
    VAO::unbind();
    Program::noProgram();
}
