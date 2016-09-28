#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include "gl_core_3_3.h"

namespace Engine {

/**
  * \class Shader
  * \brief Represents an individual shader.
  */
class Shader {
    friend class ProgramBuilder;
    public:
        enum class Type { VertexShader = GL_VERTEX_SHADER, FragmentShader = GL_FRAGMENT_SHADER,
                          GeometryShader = GL_GEOMETRY_SHADER };
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
        Shader(std::string const& file, Type t);
        GLuint m_id;
};

} // namespace Engine

#endif
