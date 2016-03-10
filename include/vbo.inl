template <class T>
void VBO::upload_data(std::vector<T> data, GLenum hint) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), hint);
    unbind();
}