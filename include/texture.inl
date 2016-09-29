template <class T>
std::vector<T> Texture::get_pixels(GLenum type, GLenum format, size_t size, GLint level) const {
    unsigned char* buf = new unsigned char[size*sizeof(T)];
    std::vector<T> vec;
    vec.reserve(size);
    bind(Tex2D);
    glGetTexImage(Tex2D, level, format, type, buf);
    for(size_t i = 0 ; i != size ; ++i) {
    }
    return vec;
}
