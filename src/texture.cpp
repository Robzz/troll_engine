#include "texture.h"
#include "image.h"
#include "debug.h"
#include <vector>

using namespace gl;
using namespace std;

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

Texture Texture::fromImage(RGBImage const& img) {
    Texture t;
    const RGBTriple* buf = nullptr;
    buf = img.getBits();
    t.texData(static_cast<int>(GL_RGB), GL_BGR, GL_UNSIGNED_BYTE,
              img.width(), img.height(), buf);
    t.generateMipmap();

    return t;
}

Texture Texture::fromImage(GreyscaleImage const& img) {
    Texture t;
    const byte* buf = nullptr;
    buf = img.getBits();
    t.texData(static_cast<int>(GL_RED), GL_RED, GL_UNSIGNED_BYTE,
              img.width(), img.height(), buf);
    t.generateMipmap();

    return t;
}

Texture Texture::depthTextureFromImage(GreyscaleImage const& img) {
    Texture t;
    vector<float> v;
    for(int i = 0 ; i != img.height() ; ++i) {
        const byte* scanline = img.getScanline(i);
        for(int j = 0 ; j != img.width() ; ++j) {
            float f = static_cast<float>(scanline[j]) / 255;
            v.push_back(f);
        }
    }
    t.texData(static_cast<int>(GL_DEPTH_COMPONENT), GL_DEPTH_COMPONENT, GL_FLOAT,
              img.width(), img.height(), v.data());

    return t;
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
        throw runtime_error("Invalid texture");
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
