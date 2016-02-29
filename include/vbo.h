#ifndef VBO_H
#define VBO_H

#include "gl_core_3_3.h"
#include <vector>

#include "debug.h"

namespace Engine {

// Class for vertex buffer objects (VBOs)
class VBO {
    public:
        VBO();
        ~VBO();

        // Bind this VBO to the specified target
        void bind(GLenum target = GL_ARRAY_BUFFER) const;
        // Unbind the buffer bound at the specified target, if any
        static void unbind(GLenum target = GL_ARRAY_BUFFER);

        // Upload data to the VBO
        template <class T>
        void upload_data(std::vector<T> data, GLenum hint = GL_STATIC_DRAW) {
            bind();
            GLV(glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), hint));
            unbind();
        }
        
    protected:
        GLuint m_id;
};

} // namespace Engine

#endif
