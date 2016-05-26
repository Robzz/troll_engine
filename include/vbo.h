/**
  * \file vbo.h
  * \brief VBO class definition
  * \author 
  */
#ifndef VBO_H
#define VBO_H

#include "gl_core_3_3.h"
#include <vector>

#include "debug.h"

/**
  * \class VBO
  * \brief Handle to a GPU Vertex Buffer %Object.
  */
class VBO {
    public:
        /**
          * \brief Default constructor.
          * Construct an empty VBO of null size.
          */
        VBO();
        /**
          * \brief Destructor.
          */
        ~VBO();

        /**
          * \brief Bind the VBO to the specified binding point.
          * \param target OpenGL binding point.
          */
        void bind(GLenum target = GL_ARRAY_BUFFER) const;
        /**
          * \brief Unbind the buffer bound at the specified target, if any
          */
        static void unbind(GLenum target = GL_ARRAY_BUFFER);

        
        /**
          * \brief Upload data to the VBO
          * \param data Data array
          * \param hint OpenGL buffer usage hint.
          */
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
