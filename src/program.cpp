#include "program.h"
#include "debug.h"

#include <algorithm>
#include <sstream>

namespace Engine {

Program* Program::s_current = nullptr;

Program::Program(GLuint id, std::vector<UniformBase*> uniforms) :
    m_id(id),
    m_uniforms(uniforms)
{ }

Program::Program(Program&& other) :
    m_id(other.m_id),
    m_uniforms(other.m_uniforms)
{
    other.m_id = 0;
}

Program& Program::operator=(Program&& other) {
    for(auto& it : m_uniforms) {
        delete it;
    }
    m_id = other.m_id;
    m_uniforms = other.m_uniforms;
    other.m_id = 0;
    return *this;
}

Program::~Program() {
    if(m_id)
        glDeleteProgram(m_id);
    for(auto& it : m_uniforms) {
        delete it;
    }
}

Program::operator bool() const {
    GLint link_status = 1;
    glGetProgramiv(m_id, GL_LINK_STATUS, &link_status);
    return link_status && std::all_of(m_uniforms.begin(), m_uniforms.end(), [](UniformBase* u) { return u->operator bool();});
}

bool Program::operator !() const {
    return !((*this).operator bool());
}


std::string Program::info_log() const {
    std::ostringstream oss;
    GLint log_length;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length);
    if(log_length > 0) {
        GLchar* buf = new GLchar[log_length];
        glGetProgramInfoLog(m_id, log_length, NULL, buf);
        delete[] buf;
    }
    else {
        for(auto& it: m_uniforms) {
            if(!(*it)) {
                oss << "Uniform " << it->name() << " not found." << std::endl;
            }
        }
    }
    return oss.str();
}

void Program::use() {
    glUseProgram(m_id);
    s_current = this;
}

void Program::noProgram() {
    glUseProgram(0);
    s_current = nullptr;
}

UniformBase* Program::getUniform(std::string const& name) {
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        if((*it)->name() == name) {
            return *it;
        }
    }
    return NULL;
}

void Program::uploadUniforms() {
    Program* previous = s_current;
    if(!is_current())
        use();
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        (*it)->upload();
    }
    if(previous != this && previous != nullptr)
        previous->use();
}

bool Program::is_current() const { return this == s_current; }

GLint Program::getAttributeLocation(std::string attribName) const {
    GLint loc = glGetAttribLocation(m_id, attribName.c_str());
    return loc;
}

GLint Program::getUniformLocation(std::string uniformName) const {
    GLint loc = glGetUniformLocation(m_id, uniformName.c_str());
    return loc;
}

Program* Program::current() { return s_current; }

UniformBase::UniformBase(GLint location, std::string const& name) :
    m_location(location),
    m_name(name),
    m_clean(false)
{ }

UniformBase::~UniformBase() {

}

UniformBase::operator bool() const {
    return m_location != -1;
}

std::string const& UniformBase::name() const {
    return m_name;
}

ProgramBuilder::ProgramBuilder() :
    m_program(),
    m_attachedShaders(),
    m_uniforms()
{
    m_program = glCreateProgram();
}

ProgramBuilder::~ProgramBuilder() { }

ProgramBuilder& ProgramBuilder::attach_shader(Shader& s) {
    glAttachShader(m_program, s.m_id);
    m_attachedShaders.push_back(&s);
    return *this;
}

ProgramBuilder& ProgramBuilder::with_uniform(std::string const& name, UniformType t) {
    m_uniforms.push_back(std::pair<std::string, UniformType>(name, t));
    return *this;
}

Program ProgramBuilder::link() {
    glLinkProgram(m_program);
    
    for(auto it = m_attachedShaders.begin() ; it != m_attachedShaders.end() ; ++it)
        glDetachShader(m_program, (*it)->m_id);

    std::vector<UniformBase*> v;
    #define TYPE(T) std::type_index(typeid(T)).hash_code
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        UniformType t = (*it).second;
        std::string const& name = (*it).first;
        GLint loc = glGetUniformLocation(m_program, name.c_str());
        if(t == UniformType::vec3) {            

            v.push_back(new Uniform<glm::vec3>(loc, name));
        }
        else if(t == UniformType::mat3) {
            v.push_back(new Uniform<glm::mat3>(loc, name));
        }
        else if(t == UniformType::mat4) {
            v.push_back(new Uniform<glm::mat4>(loc, name));
        }
        else if(t == UniformType::int_) {
            v.push_back(new Uniform<int>(loc, name));
        } 
        else if(t == UniformType::float_) {
            v.push_back(new Uniform<float>(loc, name));
        } 
        else {
            throw std::runtime_error("SHIT");
        }
    }
    #undef TYPE

    return Program(m_program, v);
}

template <>
void upload_uniform<int>(const GLint location, int const& value) {
    glUniform1i(location, value);
}

template <>
void upload_uniform<float>(const GLint location, float const& value) {
    glUniform1f(location, value);
}

template <>
void upload_uniform<glm::vec3>(const GLint location, glm::vec3 const& value) {
    glUniform3fv(location, 1, glm::value_ptr(value));
}

template <>
void upload_uniform<glm::mat3>(const GLint location, glm::mat3 const& value) {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void upload_uniform<glm::mat4>(const GLint location, glm::mat4 const& value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace Engine
