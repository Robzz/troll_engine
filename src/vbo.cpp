#include "vbo.h"

VBO::VBO() {
    GLV(glGenBuffers(1, &m_id));
}

VBO::~VBO() {
    GLV(glDeleteBuffers(1, &m_id));
}

void VBO::bind(GLenum target) const {
    GLV(glBindBuffer(target, m_id));
}

void VBO::unbind(GLenum target) {
    GLV(glBindBuffer(target, 0));
}