#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include "gl_core_3_3.h"

namespace Engine {

class Shader {
    friend class ProgramBuilder;
    public:
        enum class Type { VertexShader = GL_VERTEX_SHADER, FragmentShader = GL_FRAGMENT_SHADER,
                          GeometryShader = GL_GEOMETRY_SHADER };
        virtual ~Shader();
        operator bool() const;
        bool operator !() const;
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
