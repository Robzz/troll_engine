#include "shader.h"

#include <fstream>
#include <sstream>

#include "debug.h"

using namespace gl;

namespace Engine {

Shader::Shader(std::string const& file, gl::GLenum t) :
    m_id()
{
    // Read source and compile
    m_id = glCreateShader(t);
    std::ifstream in_file(file);
    std::stringstream ss;
    ss << in_file.rdbuf();
    std::string shader_code = ss.str();
    in_file >> shader_code;
    const char* code_ptr = shader_code.c_str();
    glShaderSource(m_id, 1, &code_ptr, NULL);
    glCompileShader(m_id);
}

Shader::~Shader() {
    glDeleteShader(m_id);
}

Shader::operator bool() const {
    GLboolean compile_status = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &compile_status);
    return compile_status == GL_TRUE;
}

bool Shader::operator !() const {
    return !((*this).operator bool());
}

std::string Shader::info_log() const {
    GLint log_length;
    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &log_length);
    if(log_length > 0) {
        GLchar* buf = new GLchar[static_cast<size_t>(log_length)];
        glGetShaderInfoLog(m_id, log_length, NULL, buf);
        std::string log(buf);
        delete[] buf;
        return log;
    }
    return "";
}

} // namespace Engine
