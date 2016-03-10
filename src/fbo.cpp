#include "fbo.h"
#include "debug.h"

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

void FBO::bind(FBO::Target const& t) {
    glBindFramebuffer(t, m_id);
}

void FBO::bind_default(FBO::Target const& t) {
    glBindFramebuffer(t, 0);
}

void FBO::attach(Target t, Attachment a, Texture& tex, GLint layer) {
    glFramebufferTexture(t, a, tex.m_id, layer);
}

void FBO::detach(FBO::Target t, FBO::Attachment a) {
    glFramebufferTexture(t, a, 0, 0);
}

bool FBO::is_complete(Target t) {
    return glCheckFramebufferStatus(t) == Complete;
}

GLenum FBO::status(Target t) {
    return glCheckFramebufferStatus(t);
}

} // namespace Engine
