#ifndef VAO_H
#define VAO_H

#include "gl_core_3_3.h"

class VBO;

class VAO {
    public:
        VAO();
        ~VAO();

        void bind() const;
        static void unbind();

        void enableVertexAttribArray(GLuint index, bool enable = true);
        void vertexAttribPointer(VBO& array, GLuint index, GLint size, GLsizei stride = 0, const GLvoid* offset = NULL, GLenum type = GL_FLOAT, bool normalize = false);

    private:
        GLuint m_id;
};

#endif
