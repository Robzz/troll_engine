#ifndef VAO_H
#define VAO_H

#include <glbinding/gl33core/gl.h>

namespace Engine {

class VBO;

/**
  * \class VAO
  * \brief Handle to a Vertex Array %Object.
  * A VAO stores the mapping between VBO data and shader program inputs.
  */
class VAO {
    public:
        /**
          * \brief Default constructor
          */
        VAO();
        /**
          * \brief Destructor
          */
        ~VAO();

        /**
          * \brief Bind the VAO.
          */
        void bind() const;
        /**
          * \brief Unbind the VAO.
          */
        static void unbind();

        /**
          * \brief Enable/disable the vertex attribute at the specified index.
          * \param index Attribute index
          * \param enable If true, enable the attribute array, disable it otherwise.
          */
        void enableVertexAttribArray(gl::GLuint index, bool enable = true);
        /**
          * \brief Specify the type and layout of the attribute data within the VBO.
          * \param array VBO containing the attribute data
          * \param index Attribute index
          * \param size Number of components in the attribute, e.g. 3 for 3-dimensional data.
          * \param stride Byte offset between two consecutive attribute values. If set to 0, the
          * attribute data is assumed to be tightly packed.
          * \param offset Byte offset between the start of the array to the start of the attribute data
          * \param type Type of the attribute component values.
          * \param normalize Specify whether the attribute values should be normalized when passed to
          * the shader program.
          */
        void vertexAttribPointer(VBO const& array, gl::GLuint index,
                                 gl::GLint size, gl::GLsizei stride = 0,
                                 const gl::GLvoid* offset = NULL,
                                 gl::GLenum type = gl::GL_FLOAT,
                                 bool normalize = false);

    private:
        gl::GLuint m_id;
};

} // namespace Engine

#endif
