#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include <glbinding/gl33core/gl.h>

namespace Engine {

/**
  * \class Shader
  * \brief Represents an individual shader.
  */
class Shader {
    friend class ProgramBuilder;
    public:
        //enum class Type {
            //VertexShader = gl::GL_VERTEX_SHADER,
            //FragmentShader = gl::GL_FRAGMENT_SHADER,
            //GeometryShader = gl::GL_GEOMETRY_SHADER
        //};

        virtual ~Shader();

        /**
          * \brief Check if the shader handle is valid.
          */
        operator bool() const;

        /**
          * \brief Check if the shader handle is invalid.
          */
        bool operator !() const;

        /**
          * \brief Return the shader information log.
          */
        std::string info_log() const;

        Shader(Shader const& other) = delete;
        Shader(Shader&& other) = delete;
        Shader& operator=(Shader const& other) = delete;
        Shader& operator=(Shader&& other) = delete;

    protected:
        Shader(std::string const& file, gl::GLenum t);
        gl::GLuint m_id;
};

} // namespace Engine

#endif
