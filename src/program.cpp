#include "program.h"
#include "debug.h"

Program::Program(GLuint id) :
    m_id(id)
{ }

Program::~Program() {
    GLV(glDeleteProgram(m_id));
}

Program::operator bool() const {
    GLint link_status = 1;
    GLV(glGetProgramiv(m_id, GL_LINK_STATUS, &link_status));
    return link_status;
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

void Program::no_program() {
    GLV(glUseProgram(0));
}

GLint Program::getAttributeLocation(std::string attribName) const {
    GLint loc = GL(glGetAttribLocation(m_id, attribName.c_str()));
    return loc;
}

ProgramBuilder::ProgramBuilder() :
    m_attachedShaders()
{
    m_program = GL(glCreateProgram());
}

ProgramBuilder::~ProgramBuilder() { }

ProgramBuilder& ProgramBuilder::attach_shader(Shader& s) {
    GLV(glAttachShader(m_program, s.m_id));
    m_attachedShaders.push_back(&s);

    return *this;
}

Program ProgramBuilder::link() {
    GLV(glLinkProgram(m_program));
    
    for(auto it = m_attachedShaders.begin() ; it != m_attachedShaders.end() ; ++it)
        GLV(glDetachShader(m_program, (*it)->m_id));

    return Program(m_program);
}
