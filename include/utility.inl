template <class T>
void dump_binary(std::vector<T> const& vec, std::string const& filename) {
    std::ofstream f(filename, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    assert(f.is_open());
    for(auto& it : vec) {
        f << it;
    }
}
