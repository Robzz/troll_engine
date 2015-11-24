#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include <GL/glew.h>

class Shader {
    friend class ProgramBuilder;
    public:
        virtual ~Shader();
        operator bool() const;
        std::string info_log() const;

    protected:
        Shader(std::istream& src_stream, GLenum shader_type);
        GLuint m_id;
};

class VertexShader : public Shader {
    public:
         VertexShader(std::istream& src_stream);
         virtual ~VertexShader();
};

class FragmentShader : public Shader {
    public:
         FragmentShader(std::istream& src_stream);
         virtual ~FragmentShader();
};

class GeometryShader : public Shader {
    public:
         GeometryShader(std::istream& src_stream);
         virtual ~GeometryShader();
};

#endif
