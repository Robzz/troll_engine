#include "fbo.h"
#include "debug.h"

FBO::FBO() :
    m_id()
{
    GLV(glGenFramebuffers(1, &m_id));
}

FBO::~FBO() {
    if(m_id)
        GLV(glDeleteFramebuffers(1, &m_id));
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
    GLV(glBindFramebuffer(t, m_id));
}

void FBO::bind_default(FBO::Target const& t) {
    GLV(glBindFramebuffer(t, 0));
}

void FBO::attach(Target t, Attachment a, Texture::Target tex_t, Texture& tex, GLint layer) {
    GLV(glFramebufferTexture2D(t, a, tex_t, tex.m_id, layer));
}

void FBO::detach(FBO::Target t, FBO::Attachment a) {
    GLV(glFramebufferTexture2D(t, a, GL_TEXTURE_2D, 0, 0));
}

bool FBO::is_complete(Target t) {
    return GL(glCheckFramebufferStatus(t)) == Complete;
}

GLenum FBO::status(Target t) {
    return GL(glCheckFramebufferStatus(t));
}
