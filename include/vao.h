#ifndef VAO_H
#define VAO_H

#include "gl_core_3_3.h"

namespace Engine {

class VBO;

// Class for vertex array objects (VAOs)
class VAO {
    public:
        VAO();
        ~VAO();

        // Bind the VAO
        void bind() const;
        // Unbind the VAO
        static void unbind();

        // Enable/disable the specified vertex attribute array
        void enableVertexAttribArray(GLuint index, bool enable = true);
        // GLVertexAttribPointer wrapper
        void vertexAttribPointer(VBO& array, GLuint index, GLint size, GLsizei stride = 0, const GLvoid* offset = NULL, GLenum type = GL_FLOAT, bool normalize = false);

    private:
        GLuint m_id;
};

} // namespace Engine

#endif
