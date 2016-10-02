#include "vbo.h"

using namespace gl;

namespace Engine {

VBO::VBO() :
    m_id()
{
    glGenBuffers(1, &m_id);
}

VBO::VBO(size_t size) :
    m_id()
{
    glGenBuffers(1, &m_id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    unbind();
}

VBO::~VBO() {
    glDeleteBuffers(1, &m_id);
}

void VBO::bind(GLenum target) const {
    glBindBuffer(target, m_id);
}

void VBO::bindBase(GLenum target, unsigned int index) {
    glBindBufferBase(target, index, m_id);
}

void VBO::unbind(GLenum target) {
    glBindBuffer(target, 0);
}

} // namespace Engine
