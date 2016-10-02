#ifndef VBO_H
#include "vbo.h"
#endif

template <class T>
void VBO::upload_data(std::vector<T> data, gl::GLenum hint) {
    bind();
    gl::glBufferData(gl::GL_ARRAY_BUFFER, static_cast<gl::GLsizeiptr>(data.size()*sizeof(T)), data.data(), hint);
    unbind();
}

template <class T>
void VBO::update_data(T const& data, size_t size, ptrdiff_t offset) {
    bind();
    gl::glBufferSubData(gl::GL_ARRAY_BUFFER, offset, size, &data);
    unbind();
}
