#include "texture.h"
#include "debug.h"
#include <vector>

using namespace gl;

namespace Engine {

Texture::Texture() :
    m_id()
{
    glGenTextures(1, &m_id);
}

Texture::Texture(Texture&& other):
    m_id(other.m_id),
    m_width(other.m_width),
    m_height(other.m_height)
{
    other.m_id = 0;
}

// TODO : do not implement this?
Texture::Texture(GLuint id) :
    m_id(id),
    m_width(),
    m_height()
{
}

Texture& Texture::operator=(Texture&& other) {
    m_id = other.m_id;
    other.m_id = 0;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
}

Texture::~Texture() {
    if(m_id)
        glDeleteTextures(1, &m_id);
}

void Texture::bind(GLenum target) const {
    glBindTexture(target, m_id);
}

void Texture::unbind(GLenum target) {
    glBindTexture(target, 0);
}

Texture Texture::noTexture() {
    return Texture(0);
}

void Texture::generateMipmap() {
    if(!m_id)
        throw std::runtime_error("Invalid texture");
    bind(GL_TEXTURE_2D);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data) {
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::filtering(GLenum minMag, GLenum filter) {
    // TODO : do not assume texture is bound
    glTexParameteri(GL_TEXTURE_2D, minMag, filter);
}

} // namespace Engine
