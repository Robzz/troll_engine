#include "texture.h"
#include "debug.h"
#include <vector>

namespace Engine {

Texture::Texture() :
    m_id()
{
    GLV(glGenTextures(1, &m_id));
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
        GLV(glDeleteTextures(1, &m_id));
}

void Texture::bind(Target target) const {
    glBindTexture(target, m_id);
}

void Texture::unbind(Target target) {
    glBindTexture(target, 0);
}

Texture Texture::noTexture() {
    return Texture(0);
}

Texture Texture::from_image(std::string const& filename) {
    Texture tex;
    tex.bind(Tex2D);
    //image::Bitmap img(image::Bitmap::readFile(filename));
    //tex.texData(GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, img.width(), img.height(), img.data().data());
    return tex;
}

void Texture::texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data) {
    GLV(glBindTexture(Tex2D, m_id));
    GLV(glTexImage2D(Tex2D, 0, internalFormat, width, height, 0, format, type, data));
    GLV(glGenerateMipmap(Tex2D));
    GLV(glBindTexture(Tex2D, 0));
}

void Texture::filtering(Filters filters, Filter f) {
    // TODO : do not assume texture is bound
    switch(filters) {
        case Magnification:
        case Minification:
            GLV(glTexParameteri(Tex2D, filters, f));
            break;
        case Both:
            GLV(glTexParameteri(Tex2D, Magnification, f));
            GLV(glTexParameteri(Tex2D, Minification, f));
            break;
    }
}

} // namespace Engine
