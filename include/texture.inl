#ifndef TEXTURE_H
#include "texture.h"
#endif

template <class T>
std::vector<T> Texture::get_pixels(gl::GLenum type, gl::GLenum format,
                                   size_t size, gl::GLint level) const {
    unsigned char* buf = new unsigned char[size*sizeof(T)];
    std::vector<T> vec;
    vec.reserve(size);
    bind(gl::GL_TEXTURE_2D);
    glGetTexImage(gl::GL_TEXTURE_2D, level, format, type, buf);
    for(size_t i = 0 ; i != size ; ++i) {
    }
    return vec;
}
