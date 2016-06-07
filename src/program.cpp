#include "program.h"
#include "utility.h"
#include "debug.h"

#include <algorithm>
#include <sstream>

namespace Engine {

const Program* Program::s_current = nullptr;

ProgramHandle::ProgramHandle(GLuint handle) :
    m_handle(handle)
{ }

ProgramHandle::~ProgramHandle() {
    glDeleteShader(m_handle);
}

GLuint ProgramHandle::value() const {
    return m_handle;
}

ShaderManager::ShaderManager(bool cache) :
    m_cache(cache),
    m_shaderCache(),
    m_programCache()
{ }

ShaderManager::~ShaderManager() {
    for(auto& p: m_shaderCache) {
        delete p.second;
    }
}

ProgramBuilder ShaderManager::buildProgram() { return ProgramBuilder(*this); }

Program::Program() :
    m_id(0),
    m_uniforms()
{ }

Program::Program(std::shared_ptr<ProgramHandle> id, std::vector<UniformBase*> uniforms) :
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
    m_uniforms = std::move(other.m_uniforms);
    other.m_id = 0;
    return *this;
}

Program::~Program() {
    for(auto& it : m_uniforms) {
        delete it;
    }
}

Program::operator bool() const {
    GLint link_status = 1;
    glGetProgramiv(m_id->value(), GL_LINK_STATUS, &link_status);
    return link_status;
    // TODO : find a less ass busting way to check this sh*t (driver optimizations and other crap)
    return link_status && std::all_of(m_uniforms.begin(), m_uniforms.end(), [](UniformBase* u) { return u->operator bool();});
}

bool Program::operator !() const {
    return !((*this).operator bool());
}

std::string Program::info_log() const {
    std::ostringstream oss;
    GLint log_length;
    glGetProgramiv(m_id->value(), GL_INFO_LOG_LENGTH, &log_length);
    if(log_length > 0) {
        GLchar* buf = new GLchar[static_cast<size_t>(log_length)];
        glGetProgramInfoLog(m_id->value(), log_length, NULL, buf);
        oss << buf << std::endl;
        delete[] buf;
    }
    for(auto& it: m_uniforms) {
        if(!(*it)) {
            oss << "Uniform " << it->name() << " not found." << std::endl;
        }
        }
    return oss.str();
}


void Program::use() const {
    glUseProgram(m_id->value());
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
    const Program* previous = s_current;
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
    GLint loc = glGetAttribLocation(m_id->value(), attribName.c_str());
    return loc;
}

GLint Program::getUniformLocation(std::string const& uniformName) const {
    GLint loc = glGetUniformLocation(m_id->value(), uniformName.c_str());
    return loc;
}

const Program* Program::current() { return s_current; }

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
    m_manager(nullptr),
    m_shaders(),
    m_uniforms()
{ }

ProgramBuilder::ProgramBuilder(ShaderManager& manager) :
    m_manager(&manager),
    m_shaders(),
    m_uniforms()
{ }

ProgramBuilder::~ProgramBuilder() { }

ProgramBuilder& ProgramBuilder::vertexShader(std::string const& file) {
    compileShader(file, Shader::Type::VertexShader);
    return *this;
}

ProgramBuilder& ProgramBuilder::fragmentShader(std::string const& file) {
    compileShader(file, Shader::Type::FragmentShader);
    return *this;
}

ProgramBuilder& ProgramBuilder::geometryShader(std::string const& file) {
    compileShader(file, Shader::Type::GeometryShader);
    return *this;
}

ProgramBuilder& ProgramBuilder::uniform(std::string const& name, UniformType t) {
    m_uniforms.push_back(std::pair<std::string, UniformType>(name, t));
    return *this;
}

Program ProgramBuilder::build() {
    auto h = std::make_shared<ProgramHandle>(glCreateProgram());
    std::set<Shader*> shaders;
    for(auto shader: m_shaders) {
        shaders.insert(shader);
        glAttachShader(h->value(), shader->m_id);
    }

    glLinkProgram(h->value());
    for(auto shader: shaders) {
        glDetachShader(h->value(), shader->m_id);
        delete shader;
    }
    std::vector<UniformBase*> v;
    #define TYPE(T) std::type_index(typeid(T)).hash_code
    for(auto it = m_uniforms.begin() ; it != m_uniforms.end() ; ++it) {
        UniformType t = (*it).second;
        std::string const& name = (*it).first;
        GLint loc = glGetUniformLocation(h->value(), name.c_str());
        if(t == UniformType::Vec3) {
            v.push_back(new Uniform<glm::vec3>(loc, name));
        }
        else if(t == UniformType::Mat3) {
            v.push_back(new Uniform<glm::mat3>(loc, name));
        }
        else if(t == UniformType::Mat4) {
            v.push_back(new Uniform<glm::mat4>(loc, name));
        }
        else if(t == UniformType::Int) {
            v.push_back(new Uniform<int>(loc, name));
        }
        else if(t == UniformType::Float) {
            v.push_back(new Uniform<float>(loc, name));
        }
        else {
            UNREACHABLE(0);
        }
    }
    #undef TYPE
    Program p(h, v);
    v.clear();
    if(!p) {
        std::ostringstream ss;
        ss << "Shader program link error." << std::endl
           << "Info:" << std::endl << p.info_log();
        throw std::runtime_error(ss.str());
    }
    if(m_manager && m_manager->m_cache)
        m_manager->m_programCache.insert(std::pair<std::set<Shader*>, std::shared_ptr<ProgramHandle>>(shaders, h));

    return p;
}

Shader* ProgramBuilder::compileShader(std::string const& file, Shader::Type t) {
    Shader* s;
    // TODO : fix duplication
    if(m_manager) {
        auto it = m_manager->m_shaderCache.find(file);
        if(it != m_manager->m_shaderCache.end()) {
            s = it->second;
        }
        else {
            s = new Shader(file, t);
            if(!*s) {
                std::ostringstream ss;
                ss << "Shader compilation error in " << file << std::endl
                   << "Info:" << std::endl << s->info_log();
                delete s;
                throw std::runtime_error(ss.str());
            }
            m_manager->m_shaderCache.insert(std::pair<std::string, Shader*>(file, s));
        }
    }
    else {
        s = new Shader(file, t);
        if(!*s) {
            std::ostringstream ss;
            ss << "Shader compilation error in " << file << std::endl
               << "Info:" << std::endl << s->info_log();
            delete s;
            throw std::runtime_error(ss.str());
        }
    }
    m_shaders.push_back(s);
    return s;
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
