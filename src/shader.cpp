#include "shader.h"

#include <iostream>

#include "debug.h"

Shader::Shader(std::istream& str_stream, GLenum shader_type) {
    m_id = GL(glCreateShader(shader_type));
}

Shader::~Shader() {
    glDeleteShader(m_id);
}
