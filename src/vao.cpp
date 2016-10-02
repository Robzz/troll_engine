#include "vao.h"
#include "vbo.h"
#include "debug.h"

using namespace gl;

namespace Engine {

VAO::VAO() :
    m_id()
{
    glGenVertexArrays(1, &m_id);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &m_id);
}

void VAO::bind() const {
    glBindVertexArray(m_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::enableVertexAttribArray(GLuint index, bool enable) {
    bind();
    if(enable)
        glEnableVertexAttribArray(index);
    else
        glDisableVertexAttribArray(index);
}

void VAO::vertexAttribPointer(VBO const& array, GLuint index, GLint size, GLsizei stride, const GLvoid* offset, GLenum type, bool normalize) {
    array.bind();
    glVertexAttribPointer(index, size, type, normalize ? GL_TRUE : GL_FALSE, stride, offset);
    VBO::unbind();
}

} // namespace Engine
