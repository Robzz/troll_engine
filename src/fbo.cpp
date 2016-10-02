#include "fbo.h"
#include "debug.h"

using namespace gl;

namespace Engine {

FBO::FBO() :
    m_id()
{
    glGenFramebuffers(1, &m_id);
}

FBO::~FBO() {
    if(m_id)
        glDeleteFramebuffers(1, &m_id);
}

FBO::FBO(FBO&& other) :
    m_id(other.m_id)
{
    other.m_id = 0;
}

FBO& FBO::operator=(FBO&& other) {
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

void FBO::bind(GLenum const& t) {
    glBindFramebuffer(t, m_id);
}

void FBO::bind_default(GLenum const& t) {
    glBindFramebuffer(t, 0);
}

void FBO::attach(GLenum t, GLenum a, Texture& tex, GLint layer) {
    glFramebufferTexture(t, a, tex.m_id, layer);
}

void FBO::detach(GLenum t, GLenum a) {
    glFramebufferTexture(t, a, 0, 0);
}

bool FBO::is_complete(GLenum t) {
    return glCheckFramebufferStatus(t) == GL_FRAMEBUFFER_COMPLETE;
}

GLenum FBO::status(GLenum t) {
    return glCheckFramebufferStatus(t);
}

} // namespace Engine
