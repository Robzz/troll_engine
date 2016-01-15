#include "vao.h"
#include "vbo.h"
#include "debug.h"

VAO::VAO() :
    m_id()
{
    GLV(glGenVertexArrays(1, &m_id));
}

VAO::~VAO() {
    GLV(glDeleteVertexArrays(1, &m_id));
}

void VAO::bind() const {
    GLV(glBindVertexArray(m_id));
}

void VAO::unbind() {
    GLV(glBindVertexArray(0));
}

void VAO::enableVertexAttribArray(GLuint index, bool enable) {
    bind();
    if(enable)
        GLV(glEnableVertexAttribArray(index))
    else
        GLV(glDisableVertexAttribArray(index));
}

void VAO::vertexAttribPointer(VBO& array, GLuint index, GLint size, GLsizei stride, const GLvoid* offset, GLenum type, bool normalize) {
    array.bind();
    GLV(glVertexAttribPointer(index, size, type, normalize ? GL_TRUE : GL_FALSE, stride, offset));
    VBO::unbind();
}
