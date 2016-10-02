#ifndef VBO_H
#define VBO_H

#include "gl_core_3_3.h"
#include <vector>

#include "debug.h"

namespace Engine {

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
         * @brief Construct an empty VBO of the specified size.
         *
         * @param size Size of the buffer, in bytes
         */
        VBO(size_t size);

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

        void bindBase(GLenum target, unsigned int index);

        /**
          * \brief Upload data to the VBO.
          * \param data Data array
          * \param hint OpenGL buffer usage hint.
          */
        template <class T>
        void upload_data(std::vector<T> data, GLenum hint = GL_STATIC_DRAW);

        /**
         * @brief Update a block of data in the buffer
         *
         * @tparam T Type of the data block to update
         * @param data Data block
         * @param size Size of the data block
         * @param offset Offset to update the buffer at
         */
        template <class T>
        void update_data(T const& data, size_t size, ptrdiff_t offset);

    protected:
        GLuint m_id;
};

#include "vbo.inl"

} // namespace Engine

#endif
