#include "texture.h"
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

Texture Texture::fromImage(Image const& img) {
    Texture t;
    const unsigned char* buf = nullptr;
    switch(img.bpp()) {
        case 24:
            buf = img.getBits();
            t.texData(static_cast<int>(GL_RGB), GL_BGR, GL_UNSIGNED_BYTE, static_cast<int>(img.width()), static_cast<int>(img.height()), buf);
            break;
        default:
            throw runtime_error("Image bpp is not 24 bits");
            break;
    }
    t.generateMipmap();

    return t;
}

Texture Texture::depthTextureFromImage(Image const& img) {
    Texture t;
    if(img.bpp() != 16 || img.type() != Image::Type::Uint16) {
        throw runtime_error("Incompatible bit depth (must be 16bit greyscale)");
    }
    vector<float> v;
    for(int i = 0 ; i != static_cast<int>(img.height()) ; ++i) {
        auto scanline = reinterpret_cast<const unsigned short*>(img.getScanLine(i));
        for(unsigned int j = 0 ; j != img.width() ; ++j) {
            float f = static_cast<float>(scanline[j]) / numeric_limits<unsigned short>::max();
            v.push_back(f);
        }
    }
    GLint alignment, swap_bytes;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swap_bytes);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    //glPixelStorei(GL_UNPACK_SWAP_BYTES, 1);
    t.texData(static_cast<int>(GL_DEPTH_COMPONENT16), GL_DEPTH_COMPONENT, GL_FLOAT, static_cast<int>(img.width()), static_cast<int>(img.height()), v.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, swap_bytes);

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
