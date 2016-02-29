#include "shader.h"

#include <iostream>

#include "debug.h"

namespace Engine {

Shader::Shader(std::istream& str_stream, GLenum shader_type) :
    m_id()
{
    // Read source and compile
    m_id = GL(glCreateShader(shader_type));
    std::string source((std::istreambuf_iterator<char>(str_stream)), (std::istreambuf_iterator<char>()));
    const char* source_ptr = source.c_str();
    GLV(glShaderSource(m_id, 1, &source_ptr, NULL));
    GLV(glCompileShader(m_id));

    // Check for errors
}

Shader::~Shader() {
    GLV(glDeleteShader(m_id));
}

Shader::operator bool() const {
    GLint compile_status = 0;
    GLV(glGetShaderiv(m_id, GL_COMPILE_STATUS, &compile_status));
    return compile_status == GL_TRUE;
}

bool Shader::operator !() const {
    return !((*this).operator bool());
}

std::string Shader::info_log() const {
    GLint log_length;
    GLV(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &log_length));
    if(log_length > 0) {
        GLchar* buf = new GLchar[log_length];
        GLV(glGetShaderInfoLog(m_id, log_length, NULL, buf));
        std::string log(buf);
        delete[] buf;
        return log;
    }
    return "";
}

VertexShader::VertexShader(std::istream& src_stream) :
    Shader(src_stream, GL_VERTEX_SHADER)
{ }

VertexShader::~VertexShader() { }

FragmentShader::FragmentShader(std::istream& src_stream) :
    Shader(src_stream, GL_FRAGMENT_SHADER)
{ }

FragmentShader::~FragmentShader() { }

GeometryShader::GeometryShader(std::istream& src_stream) :
    Shader(src_stream, GL_GEOMETRY_SHADER)
{ }

GeometryShader::~GeometryShader() { }

} // namespace Engine
