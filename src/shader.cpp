#include "shader.h"

#include <iostream>

#include "debug.h"

Shader::Shader(std::istream& str_stream, GLenum shader_type) {
    // Read source and compile
    m_id = GL(glCreateShader(shader_type));
    std::string source((std::istreambuf_iterator<char>(str_stream)), (std::istreambuf_iterator<char>()));
    const char* source_ptr = source.c_str();
    GL(glShaderSource(m_id, 1, &source_ptr, NULL));
    GL(glCompileShader(m_id));

    // Check for errors
}

Shader::~Shader() {
    GL(glDeleteShader(m_id));
}

Shader::operator bool() const {
    GLint compile_status;
    GL(glGetShaderiv(m_id, GL_COMPILE_STATUS, &compile_status));
    return compile_status;
}

std::string Shader::info_log() const {
    GLint log_length;
    GL(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &log_length));
    GLchar* buf = new GLchar[log_length];
    GL(glGetShaderInfoLog(m_id, log_length, NULL, buf));
    std::string log(buf);
    delete[] buf;
    return log;
}

VertexShader::VertexShader(std::istream& src_stream) :
    Shader(src_stream, GL_VERTEX_SHADER)
{ }

VertexShader::~VertexShader() {
    GL(glDeleteShader(m_id));
}

FragmentShader::FragmentShader(std::istream& src_stream) :
    Shader(src_stream, GL_FRAGMENT_SHADER)
{ }

FragmentShader::~FragmentShader() {
    GL(glDeleteShader(m_id));
}

GeometryShader::GeometryShader(std::istream& src_stream) :
    Shader(src_stream, GL_GEOMETRY_SHADER)
{ }

GeometryShader::~GeometryShader() {
    GL(glDeleteShader(m_id));
}

