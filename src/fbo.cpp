#include "fbo.h"
#include "debug.h"

namespace Engine {

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

void FBO::attach(Target t, Attachment a, Texture& tex, GLint layer) {
    GLV(glFramebufferTexture(t, a, tex.m_id, layer));
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

std::vector<unsigned char> FBO::readPixels(Format f, Type t, GLsizei width, GLsizei height, int x, int y) {
    int n_components   = (f == Rgb)   ? 3 :
                         (f == Bgr)   ? 3 :
                         (f == Rgba)  ? 4 : 1,
        component_size = (t == Ubyte) ? 1 : 4;
    std::vector<unsigned char> vec(width * height * n_components * component_size, 0);
    GLV(glReadPixels(x, y, width, height, f, t, vec.data()));
    assert(vec.size() == (width * height * n_components * component_size));
    return vec;
}

} // namespace Engine
