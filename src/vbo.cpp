#include "vbo.h"

namespace Engine {

VBO::VBO() :
    m_id()
{
    glGenBuffers(1, &m_id);
}

VBO::~VBO() {
    glDeleteBuffers(1, &m_id);
}

void VBO::bind(GLenum target) const {
    glBindBuffer(target, m_id);
}

void VBO::unbind(GLenum target) {
    glBindBuffer(target, 0);
}

} // namespace Engine
