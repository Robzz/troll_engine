#include "texture.h"
#include "debug.h"
#include "bitmap.h"
#include <vector>


Texture::Texture() :
    m_id()
{
    GLV(glGenTextures(1, &m_id));
}

Texture::Texture(Texture&& other):
    m_id(other.m_id)
{ }

Texture::Texture(GLuint id) :
    m_id(id)
{ }

Texture& Texture::operator=(Texture&& other) {
    m_id = other.m_id;
    return *this;
}

Texture::~Texture() {
    if(m_id)
        GLV(glDeleteTextures(1, &m_id));
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

Texture Texture::from_image(std::string const& filename) {
    Texture tex;
    tex.bind(GL_TEXTURE_2D);
    GLV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    image::Bitmap img(image::Bitmap::readFile(filename));
    tex.texData(GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, img.width(), img.height(), img.data().data());
    return tex;
}

void Texture::texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data) {
    GLV(glBindTexture(GL_TEXTURE_2D, m_id));
    GLV(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data));
    GLV(glGenerateMipmap(GL_TEXTURE_2D));
    GLV(glBindTexture(GL_TEXTURE_2D, 0));
}
