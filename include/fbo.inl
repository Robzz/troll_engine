#ifndef FBO_H
#include "fbo.h"
#endif

template <class T>
std::vector<T> FBO::readPixels(gl::GLenum f, gl::GLenum t, gl::GLsizei width, gl::GLsizei height, int x, int y) {
    int n_components   = (f == gl::GL_RGB)             ? 3 :
                         (f == gl::GL_BGR)             ? 3 :
                         (f == gl::GL_RGBA)            ? 4 :
                         (f == gl::GL_DEPTH_COMPONENT) ? 1 : UNREACHABLE(0),
        component_size = (t == gl::GL_BYTE)   ? 1 :
                         (t == gl::GL_SHORT) ? 2 :
                         (t == gl::GL_FLOAT)  ? 4 : UNREACHABLE(0);
    size_t vec_size = static_cast<size_t>(width * height * n_components * component_size);
    std::vector<T> vec(vec_size, 0);
    glReadPixels(x, y, width, height, f, t, vec.data());
    assert(vec.size() == (vec_size));
    return vec;
}
