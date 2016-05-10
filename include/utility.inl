template <class T>
void dump_binary(std::vector<T> const& vec, std::string const& filename) {
    std::ofstream f(filename, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    assert(f.is_open());
    for(auto& it : vec) {
        f << it;
    }
}

template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type
operator|(E e1, E e2) {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(static_cast<underlying>(e1) | static_cast<underlying>(e2));
}

template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type
operator&(E e1, E e2) {
    typedef typename std::underlying_type<E>::type underlying;
    return static_cast<E>(static_cast<underlying>(e1) & static_cast<underlying>(e2));
}
