#ifndef VBO_H
#define VBO_H

#include "gl_core_3_3.h"
#include <vector>

#include "debug.h"

class VBO {
    public:
        VBO();
        ~VBO();

        void bind(GLenum target = GL_ARRAY_BUFFER) const;
        static void unbind(GLenum target = GL_ARRAY_BUFFER);

        template <class T>
        void upload_data(std::vector<T> data, GLenum hint = GL_STATIC_DRAW) {
            bind();
            GLV(glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), hint));
            unbind();
        }
        
    protected:
        GLuint m_id;
};

#endif
