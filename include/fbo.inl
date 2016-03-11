template <class T>
std::vector<T> FBO::readPixels(Format f, Type t, GLsizei width, GLsizei height, int x, int y) {
    int n_components   = (f == Rgb)            ? 3 :
                         (f == Bgr)            ? 3 :
                         (f == Rgba)           ? 4 :
                         (f == DepthComponent) ? 1 : UNREACHABLE(0),
        component_size = (t == Ubyte)  ? 1 : 
                         (t == Ushort) ? 2 :
                         (t == Float)  ? 4 : UNREACHABLE(0);
    size_t vec_size = static_cast<size_t>(width * height * n_components * component_size);
    std::vector<T> vec(vec_size, 0);
    glReadPixels(x, y, width, height, f, t, vec.data());
    assert(vec.size() == (vec_size));
    return vec;
}
