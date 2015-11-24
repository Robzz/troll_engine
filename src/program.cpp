#include "program.h"
#include "debug.h"

Program::Program(GLuint id) :
    m_id(id)
{ }

Program::~Program() {
    GL(glDeleteProgram(m_id));
}

Program::operator bool() const {
    GLint link_status;
    GL(glGetProgramiv(m_id, GL_LINK_STATUS, &link_status));
    return link_status;
}

std::string Program::info_log() const {
    GLint log_length;
    GL(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length));
    GLchar* buf = new GLchar[log_length];
    GL(glGetProgramInfoLog(m_id, log_length, NULL, buf));
    std::string log(buf);
    delete[] buf;
    return log;
}

void Program::use() const {
    GL(glUseProgram(m_id));
}

void Program::no_program() {
    GL(glUseProgram(0));
}

ProgramBuilder::ProgramBuilder() :
    m_attachedShaders()
{
    m_program = GL(glCreateProgram());
}

ProgramBuilder::~ProgramBuilder() { }

ProgramBuilder& ProgramBuilder::attach_shader(Shader& s) {
    GL(glAttachShader(m_program, s.m_id));
    m_attachedShaders.push_back(&s);

    return *this;
}

Program ProgramBuilder::link() {
    GL(glLinkProgram(m_program));
    
    for(auto it = m_attachedShaders.begin() ; it != m_attachedShaders.end() ; ++it)
        GL(glDetachShader(m_program, (*it)->m_id));

    return Program(m_program);
}
