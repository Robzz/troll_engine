#include "program.h"
#include "debug.h"

#include <algorithm>



Program::Program(GLuint id, std::vector<UniformBase*> uniforms) :
    m_id(id),
    m_uniforms(uniforms)
{ }

Program::~Program() {
    GLV(glDeleteProgram(m_id));
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        delete *it;
    }
}

Program::operator bool() const {
    GLint link_status = 1;
    GLV(glGetProgramiv(m_id, GL_LINK_STATUS, &link_status));
    return link_status && std::all_of(m_uniforms.begin(), m_uniforms.end(), [](UniformBase* u) { return u->operator bool();});
}

bool Program::operator !() const {
    return !((*this).operator bool());
}


std::string Program::info_log() const {
    GLint log_length;
    GLV(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length));
    GLchar* buf = new GLchar[log_length];
    GLV(glGetProgramInfoLog(m_id, log_length, NULL, buf));
    std::string log(buf);
    delete[] buf;
    return log;
}

void Program::use() const {
    GLV(glUseProgram(m_id));
}

void Program::noProgram() {
    GLV(glUseProgram(0));
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
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        (*it)->upload();
    }
}

GLint Program::getAttributeLocation(std::string attribName) const {
    GLint loc = GL(glGetAttribLocation(m_id, attribName.c_str()));
    return loc;
}

GLint Program::getUniformLocation(std::string uniformName) const {
    GLint loc = GL(glGetUniformLocation(m_id, uniformName.c_str()));
    return loc;
}

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
    m_program = GL(glCreateProgram());
}

ProgramBuilder::~ProgramBuilder() { }

ProgramBuilder& ProgramBuilder::attach_shader(Shader& s) {
    GLV(glAttachShader(m_program, s.m_id));
    m_attachedShaders.push_back(&s);
    return *this;
}

ProgramBuilder& ProgramBuilder::with_uniform(std::string const& name, UniformType t) {
    m_uniforms.push_back(std::pair<std::string, UniformType>(name, t));
    return *this;
}

Program ProgramBuilder::link() {
    GLV(glLinkProgram(m_program));
    
    for(auto it = m_attachedShaders.begin() ; it != m_attachedShaders.end() ; ++it)
        GLV(glDetachShader(m_program, (*it)->m_id));

    std::vector<UniformBase*> v;
    #define TYPE(T) std::type_index(typeid(T)).hash_code
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        UniformType t = (*it).second;
        std::string const& name = (*it).first;
        GLint loc = GL(glGetUniformLocation(m_program, name.c_str()));
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
