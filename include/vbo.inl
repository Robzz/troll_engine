template <class T>
void VBO::upload_data(std::vector<T> data, GLenum hint) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size()*sizeof(T)), data.data(), hint);
    unbind();
}

template <class T>
void VBO::update_data(T const& data, size_t size, ptrdiff_t offset) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, &data);
    unbind();
}
